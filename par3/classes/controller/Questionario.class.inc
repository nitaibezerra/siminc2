<?php

/**
 * Par_Controle_Questionario
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-09-2015
 * @link     no link
 */
class Par3_Controller_Questionario extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }

    public function pegaQrpid($inuid, $queid, $estuf, $muncod = null)
    {
    	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    	$modelQuestionario = new Par3_Model_Questionario($_REQUEST['inuid']);
    	$modelInstrumento = new Par3_Model_InstrumentoUnidade($inuid);

    	$qrpid = $modelQuestionario->pegaQuestionario($inuid, $queid);

    	if (!$qrpid)
    	{
	       	$sql = "SELECT CASE WHEN itrid IN (1, 3) THEN e.estdescricao ELSE m.mundescricao END as descricao
    	              FROM par.instrumentounidade i
    	         LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
    	         LEFT JOIN territorios.estado    e ON e.estuf  = i.estuf
    	             WHERE inuid = {$inuid}";

	       	$titulo = $modelInstrumento->pegaUm($sql);

			$arParam = array ( "queid" => $queid, "titulo" => "PAR 2010 (".$titulo.")" );

			$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );

			//$sql = "INSERT INTO par.parquestionario (inuid, qrpid) VALUES ({$inuid}, {$qrpid})";
			//$db->executar( $sql );
			//$db->commit();
    	}

        return $qrpid;
    }
}//end class

?>