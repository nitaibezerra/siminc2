<?php

// carrega as funчѕes gerais
// include_once "config.inc";
// include_once APPRAIZ . "includes/classes_simec.inc";
// include_once "_constantes.php";
// include_once '_funcoes.php';
// include_once '_funcoesPar.php';
// include_once APPRAIZ . "includes/funcoes.inc";
// require_once APPRAIZ . "includes/classes/dateTime.inc";
// require_once APPRAIZ . "includes/classes/Controle.class.inc";
// require_once APPRAIZ . "includes/classes/Visao.class.inc";
// require_once APPRAIZ . "includes/classes/Modelo.class.inc";
// include_once 'autoload.php';
// // atualiza aчуo do usuсrio no sistema
// include APPRAIZ . "includes/registraracesso.php";

class Ajax {

    public $db;

    public function __construct($db = false) {
        if ($db) {
            $this->db = new cls_banco();
        }
    }

   
    /**
     * Mщtodo que verifica se existe instrumento Unidade e grava na sessуo, senуo existe ele grava e coloca na sessуo
     * @param array $post
     * @return
     */
    public function verificaInstrumentoUnidade($post) {
        $inuid = $post['inuid'];
        if (!$inuid) {
            $obEntidadeParControle = new EntidadeParControle();
            $inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
        }
        if ( $inuid && $inuid!='undefined' ) {
        	
        	if( !$this->db ){
        		$this->db = new cls_banco();
        	}
        	
            $sql = "SELECT itrid FROM par.instrumentounidade WHERE inuid = $inuid";
            
            $_SESSION['par']['itrid'] = $this->db->pegaUm($sql);
            if ($post['estuf'] == 'DF') {
                $_SESSION['par']['itrid'] = 1;
            }
            $_SESSION['par']['inuid'] = $inuid;
            $_SESSION['par']['estuf'] = $post['estuf'];
            $_SESSION['par']['muncod'] = $post['muncod'];
        }
        echo $_SESSION['par']['inuid'];
        die;
    }

}

if (isset($_POST['requisicao'])) {
    $db = ( isset($_POST['db']) && !empty($_POST['db']) ) ? true : false;
    $obAjax = new Ajax($db);
    $obAjax->$_POST['requisicao']($_POST);
}
?>