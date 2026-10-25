const issueForm = document.getElementById("issue-form");

issueForm.addEventListener("submit", function (event) {
    event.preventDefault();

    const type = document.getElementById("issue-type").value;
    const title = document.getElementById("issue-title").value.trim();
    const description = document.getElementById("issue-description").value.trim();
    const version = document.getElementById("issue-version").value.trim();
    const system = document.getElementById("issue-system").value.trim();

    if (!title || !description) {
        alert("Please enter an issue title and description.");
        return;
    }

    let issueTitle = title;

    if (type === "bug") {
        issueTitle = `[Bug]: ${title}`;
    } else if (type === "feature") {
        issueTitle = `[Feature Request]: ${title}`;
    } else {
        issueTitle = `[Other]: ${title}`;
    }

    let issueBody = description;

    if (version) {
        issueBody += `\n\n## MiniMiku version\n${version}`;
    }

    if (system) {
        issueBody += `\n\n## System information\n${system}`;
    }

    const params = new URLSearchParams({
        title: issueTitle,
        body: issueBody
    });

    const issueURL =
        `https://github.com/GDDASH45/minimiku/issues/new?${params.toString()}`;

    window.location.href = issueURL;
});