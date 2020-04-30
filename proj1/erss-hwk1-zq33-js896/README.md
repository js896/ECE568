**IMPORTANT**
**IMPORTANT**
**IMPORTANT**
Whenever you need to create or modify an order, for the arrival time, the format is YYYY-MM-DD HH:MM:SS
For example: 2019-01-02 12:00:00

**How To Run The Project:**

*Docker Installation*

sudo apt-get install docker.io  docker-compose

*Run the server*

sudo docker-compose up

**Function Introduction**
1.
In the welcome page (http://vcm-12399.vm.duke.edu:8000/)
There are 2 options you can choose either sign up of sign in.
If you press sign up, you will be directed to account creation page, input the required information and press the "Yes" button you will be redirected to log in page (http://vcm-12399.vm.duke.edu:8000/accounts/login/?next=/request/). 

2.
In the login page input the username and password you just signed up with. Then you will be directed to the page (http://vcm-12399.vm.duke.edu:8000/request/). You logged in successfully.

3.
If you try to log in with wrong username or password, you will get the message " Wrong username or password. Please try again. " at the top.

4.
From any session you can log out.

5.
After logging, you can choose to log in as either driver or customor, when you choose to log in as driver, press profile detail to save personal & vehicle information. Then you will be directed to a page showing the driver information.

6.
Press "My driver orders", then press "Update profile" in the middle, you will be able to update the driver's information.

7.
Press "My owner Orders" at the top left, input the required information then you will be able to submit a request. Note the "Date and time of arrival" should be in the format "yyyy-mm-dd hh-mm-ss".

8.
After submitting the request, you will be directed to a page specifying the non-complete ride they belong to.

9.
In page of 8, press Edit in the middle, then you will be able to edit the ride information.

10.
You can edit the requested attributes of a ride if you like. But after entering driver information that satisfies the customer order, and pressing  "Find New Orders" in the driver page, you can find orders that the driver can take, press detail and in the new page press "Confirm this Order", the driver confirmed the order and the customer won't be able to edit it anymore. And you can see in the page (URL: http://vcm-12399.vm.duke.edu:8000/request/),the "status" has become "confirmed".

11.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/), if you press detail, you canview all ride details for any open ride they belong to.

12.
Same as above, but do it after the status has beome "confirmed".

13.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/), press "Join An Order", then input your "destination", "Earliest Arrival Time" and "Latest Arrival Time", note the time format should be "yyyy-mm-dd hh-mm-ss".

14.
There are available orders acoording to your input information, then you can join them(Provided that the customer is willing to sharei).

15.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/), press "My dirver Orders" on the top left, then you will be directed to the page (URL: http://vcm-12399.vm.duke.edu:8000/request/driver/myOrders), in that page press "Find New Orders", then you will be able to search for open ride requests.

16.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/driver/myOrders), press "Find New Orders". In the available orders, choose one, press detail in the next page you can confirm it.

17.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/driver/myOrders), you can see that.

18.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/driver/myOrders), you can see that, choose one confirmed order and press detail, then you can see that.

19.
In the page (URL: http://vcm-12399.vm.duke.edu:8000/request/driver/myOrders), you can see that, choose one confirmed order and press detail, in the next page press "Complete this order", in the next page press "Yes", then you can edit this order after the order is confirmed.
