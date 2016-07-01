/**
    * Builds the home page (browse tickets).
    * 
    * @access public
    * @return void
    * @param array $request
    */
    public function buildHome($request) {
        // Retrieve list of tickets from DB
        $tickets = FormHandler::get_tickets('Admin', $request , 0);
        parent::buildHome($request, $tickets);
    }