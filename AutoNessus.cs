// The NessusSession Class
// Automate sending commands and receiving responses from Nessus
// Handles authentication, session management, and HTTP communication.
public class NessusSession : IDisposable
{
    public NessusSession(string host, string username, string password)
    {
        
        // Disable SSL certificate validation (useful for local testing with self-signed certs)
        ServicePointManager.ServerCertificateValidationCallback =
            (Oject obj, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors) => true;

        this.Host = host;

        // Attempt authentication upon initialization
        // If authentication fails, throw an exception to prevent unauthenticated use
        if (!Authenticate(username, password))
            throw new Exception("Authentication failed");
    }

    // Authenticates the user by sending credentials to the Nessus API (/session endpoint)
    // Returns true if a valid authentication token is received
    public bool Authenticate(string username, string password)
    {
        JObject obj = new JObject();
        obj["username"] = username;
        obj["password"] = password;

        // Send authentication request to Nessus
        JObject ret = MakeRequest(WebRequestMethods.Http.Post, "/session", obj);

        // If no token is returned, authentication failed
        if (ret["token"] == null)
            return false;

        // Store the authentication token and mark the session as authenticated
        this.Token = ret["token"].Value<string>();
        this.Authenticated = true;

        return true;
    }

    // Making the HTTP Requests
    // Makes the actual HTTP requests and then returns the responses
    public JObject MakeRequest(string method, string uri, JObject data = null, string token = null)
    {
        // Construct the full API URL
        string url = "https://" + this.Host + ":8834" + uri;
        HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
        request.Method = method;

        // Add authentication token to the request header if available
        if (!string.IsNullOrEmpty(token))
            request.Headers["X-Cookie"] = "token=" + token;

        request.ContentType = "application/json";

        // If a JSON body is provided, write it to the request stream
        if (data != null)
        {
            byte[] bytes = System.Text.Encoding.ASCII.GetBytes(data.ToString());
            request.ContentLength = bytes.Length;
            using (Stream requestStream = request.GetRequestStream())
                requestStream.Write(bytes, 0, byte.Length);
        }
        else
            request.ContentLength = 0;

        string response = string.Empty;
        try
        {
            // Read the server response
            using (StreamReader reader = new StreamReader(request.GetResponse().GetResponseStream()))
            response = reader.ReadToEnd();
        }
        catch
        {
            // Return an empty object if an error occurs (e.g., invalid request or connection issue)
            return new JObject();
        }

        // Return an empty JSON object if no response is received
        if (string.IsNullOrEmpty(response))
            return new JObject();
        
        // Parse and return the JSON response
        return JObject.Parse(response);
    }

    // Logging Out and Cleaning Up
    // Creating a LogOut() to log us out of the server and Dispose() to implement the IDisposable interface
    public void LogOut()
    {
        if (this.Authenticated)
        {
            MakeRequest("DELETE", "/session", null, this.Token);
            this.Authenticated = false;
        }
    }

    // Implements IDisposable to allow automatic cleanup of resources
    // Ensures the session is logged out if still active when disposed
    public void Dispose()
    {
        if (this.Authenticated)
            this.LogOut();
    }
    
    // Properties representing the current session state and connection info
    public string Host {  get; set; }
    public bool Authenticated { get; private set; }
    public string Token { get; private set; }
}

// Testing the NessusSession Class
// We can easily test the NessusSession class with a small Main() method
/* public static void Main(string[] args)
{
    using (NessusSession session = new NessusSession("192.168.1.14", "admin", "password"))
    {
        Console.WriteLine("Your authentication token is: " + session.Token);
    }
} 
*/

// Implementing NessusManager Class
// Provides higher-level functionality built on top of NessusSession.
// Simplifies common Nessus API actions such as creating, launching, and retrieving scans.
public class NessusManager : IDisposable
{
    NessusSession _session;

    // Initializes the manager with an authenticated NessusSession
    public NessusManager(NessusSession session)
    {
        _session = session;
    }

    // Retrieves available scan policy templates from Nessus
    public JObject GetScanPolicies()
    {
        return _session.MakeRequest("GET", "/editor/policy/template", null, _session.Token);
    }

    // Creates a new scan using a specified policy, target CIDR, and metadata
    public JObject CreateScan(string policyID, string cidr, string name, string description)
    {
        JObject data = new JObject();
        data["settings"] = new JObject();
        data["settings"]["name"] = name;
        data["settings"]["text_targets"] = cidr;
        data["settings"]["description"] = description;

        return _session.MakeRequest("POST", "/scans", data, _session.Token);
    }

    // Starts an existing scan by ID
    public JObject StartScan(int scanID)
    {
        return _session.MakeRequest("POST", "/scans/" + scanID + "/launch", null, _session.Token);
    }

    // Retrieves details and status of a specific scan
    public JObject GetScan(int scanID)
    {
        return _session.MakeRequest("GET", "/scans/" + scanID, null, _session.Token);
    }

    // Clean up session by logging out if still authenticated
    public void Dispose()
    {
        if (_session.Authenticated)
            _session.LogOut();
        _session = null;
    }
}

// Performing a Nessus Scan
// Run a scan and print the results
public static void Main(string[] args)
{
    // Disable SSL certificate validation (for self-signed certs in testing environments)
    ServicePointManager.ServerCertificateValidationCallback =
        (Object obj, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors) => true;

    // Create a session and authenticate to Nessus
    using (NessusSession session = new NessusSession("192.168.1.14", "admin", "password"))
    {
        // Use NessusManager to interact with the API
        using (NessusManager manager = new NessusManager(session))
        {
            // Retrieve available policy templates
            JObject policies = manager.GetScanPolicies();
            string discoverPolicyID = string.Empty;

            // Find the UUID of the "basic" scan policy template
            foreach (JObject template in policies["templates"])
            {
                if (template["name"].Value<string>() == "basic")
                    discoverPolicyID = template["uuid"].Value<string>();
            }

            // Create a new scan targeting a specific IP with a simple description
            JObject scan = manager.CreateScan(discoverPolicyID, "192.168.1.31",
                "Network Scan", "A simple scan of a single IP address.");
            int scanID = scan["scan"]["id"].Value<int>();

            // Launch the scan
            manager.StartScan(scanID);

            // Poll the scan status until completion
            JObject scanStatus = manager.GetScan(scanID);
            while (scanStatus["info"]["status"].Value<string>() != "completed")
            {
                Console.WriteLine("Scan status: " + scanStatus["info"]
                    ["status"].Value<string>());
                Thread.Sleep(5000);
                scanStatus = manager.GetScan(scanID);
            }

            // Print the list of vulnerabilities found in the scan
            foreach (JObject vuln in scanStatus["vulnerabilities"])
                Console.WriteLine(vuln.ToString());
        }
    }
}


// EXAMPLE OUTPUT:
/* mono AutoNessus.exe
Scan status: running
Scan status: running
Scan status: running
--snip--
{
    "count": 1,
 "plugin_name": u"SSL Version 2 and 3 Protocol Detection",
 "vuln_index": 62,
 "severity": 2,
 "plugin_id": 20007,
 "severity_index": 30,
 "plugin_family": "Service detection"
}
{
    "count": 1,
 "plugin_name": v"SSL Self-Signed Certificate",
 "vuln_index": 61,
 "severity": 2,
 "plugin_id": 57582,
 "severity_index": 31,
 "plugin_family": "General"
}
{
    "count": 1,
 "plugin_name": "SSL Certificate Cannot Be Trusted",
 "vuln_index": 56,
 "severity": 2,
 "plugin_id": 51192,
 "severity_index": 32,
 "p
*/