<?php
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

/**
 * Class Sms
 * @description : Classe manipuladora de envios SMS
 * @version 1.0
 * @author Orion Teles de Mesquita
 * @since 28/11/2013
 */
class Sms{

    public function enviarSms($aCelulares, $conteudo, $data = null, $sisid = null)
    {
        $aCelulares = (array) $aCelulares;

        $db = new cls_banco();

        if(strlen($conteudo) > 150){
            $conteudo = str_replace($conteudo, 0, 150);
        }

        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        if($data){
            $tipo = HumanMultipleSend::TYPE_E;
            $schedule = date('d/m/Y H:i:s', strtotime($data));
        } else {
            $schedule = '';
            $data = date('Y-m-d H:i:s');
            $tipo = HumanMultipleSend::TYPE_A;
        }

        $aMensagem = array();
        foreach ($aCelulares as $celular) {
        	$celular = str_replace(array('(', ')', '-', ' ', '_', '/', '.'), array(''), $celular);
            if(in_array(substr($celular, 4, 1), array(7, 8, 9))){
                $id = $this->gravarHistorico($celular, $conteudo, $sisid);
                $aMensagem[] = $schedule ? "{$celular};{$conteudo};$id;;$schedule" : "{$celular};{$conteudo}";
            }
        }

        $msg_list  = implode("\n", $aMensagem);

        // Envia SMS
        $humanMultipleSend = new HumanMultipleSend("mec.gway", "WWh4O8xZOo");
        $responses = $humanMultipleSend->sendMultipleList($tipo, $msg_list);

//        foreach ($responses as $response) {
//            $codigo = $response->getCode();
//        }
    }

    protected function gravarHistorico($celular, $conteudo, $sisid)
    {
        $db = new cls_banco();

        $data = date('Y-m-d H:i:s');

        $sql = "INSERT INTO seguranca.sms(
		            sisid, smscelular, smsmensagem, smsdataenvio)
				    VALUES ('$sisid', '$celular', '$conteudo', '$data')
				    returning smsid;
			";
        $id = $db->pegaUm($sql);
        $db->commit();
        return $id;
    }

}
?>