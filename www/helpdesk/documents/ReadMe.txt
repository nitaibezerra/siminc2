
ReadMe
----------------------------------------------------------------------------

Triangle Solutions Ltd has developed an application which allows you to offer
your web site users a way to request Support Tickets.

*****************************
PHPSupportTickets version 2.2
*****************************

This system enables users of your company to request support on your products and
services and to receive this support through a highly usable and intuitive web
interface. The application can be entirely customised to your needs, with the
creation of departments representing the different areas of customer support, and the
small amount of data required by users in order to register.

Features
----------------------------------------------------------------------------
+  There are 3 types of users: Admins, Moderators (mods) and Clients
+  Moderators are the users who mainly answer Client's Tickets.
+  All users can post Tickets.
+  Tickets are assigned to a Department.
+  Mods are assigned to a Department. In future versions they will be able to be
   assigned to several Departments.
+  Admins and Clients do not belong to a Department.
+  All users can post Answers to Tickets.
+  All users can attach 1 file to each Ticket or Answer.
+  Admins can edit site options, such as emailing preferences.
+  Clients can self-register.
+  Admins can create other users of all 3 types.
+  Admins can create, edit and delete Departments.
+  Mods can only see tickets they have posted and tickets in their Department(s)
+  Clients can only see tickets they have posted.
+  Mods can view their Department's info and other Mods.
+  Mods are automatically emailed when a new Ticket is posted.
+  Users are automatically emailed when an Answer to their Ticket is posted.
+  All users can open/close Tickets.
+  Admins can re-assign Tickets to a different Department (later release)
+  Refresh never creates duplicates of Tickets, Answers or Users (using Timestamp)
+  Pagination is used on all browsing pages
+  Vast possibilities of extension, such as a rating system, where Clients rate the
   mods' and admins' answers to their Tickets.

Requirements
----------------------------------------------------------------------------

+ Requires PHP v5.0+ - For the security measures incorporated in this release.
+ Mysql 4 or later is MANDATORY, Mysql 5.0 is recommended
+ File Uploads Switched On - for attachments.
+ Linux or Windows, untested on Macintosh.
+ mysqli extension enabled or mysql (if using mysql database)
