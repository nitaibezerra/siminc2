<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 18/06/2015
 * Time: 16:08
 */

class WorkflowConjur {
    public static function preAcaoDefinePrazoAdvogado(){
        global $db;

        $hadid         = $_POST['hadid'];
        $haddtprazoadv = $_POST['haddtprazoadv'];

        if (!$haddtprazoadv)
            exit( simec_json_encode(array('boo' => false, 'msg' => 'Prazo do Advogado não pode ficar em branco.')) );
        if (!$hadid)
            exit( simec_json_encode(array('boo' => false, 'msg' => 'Histórico do Advogado não encontrado, verifique se existe um advogado associado ao processo.')) );

        $sql = "update conjur.historicoadvogados set
                    haddtprazoadv=".(($haddtprazoadv)?"'".formata_data_sql($haddtprazoadv)."'":"NULL")."
                where hadid = {$hadid}";
        $db->executar($sql);


        if($db->commit())
            exit( simec_json_encode(array('boo' => true, 'msg' => '')) );
        else
            exit( simec_json_encode(array('boo' => false, 'msg' => 'Erro na tramitação, tente mais tarde.')) );

    }
}