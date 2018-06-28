<?php

class Public_Controller_Objetivoppa
{
    public function salvar($dados)
    {
        $url = "?modulo=apoio/objetivoppa_form&acao=A";
        try {
            $mdObjetivoppa = new Public_Model_ObjetivoPpa($dados['oppid']);
            $mdObjetivoppa->popularDadosObjeto($dados);
            $mdObjetivoppa->prsano = $mdObjetivoppa->prsano ? $mdObjetivoppa->prsano : $_SESSION['exercicio'];
            $mdObjetivoppa->salvar(null,null,['prsano', 'oppdsc',  'oppnome', 'oppcod', 'oppstatus']);
            $this->salvarMetas($mdObjetivoppa->oppid);
            $mdObjetivoppa->commit();
            simec_redirecionar($url."&oppid=".$dados['oppid'], 'success');
        } catch (Exception $e){
            $mdObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function salvarMetas($oppid){
        $mdObjetivoMetappa = new Public_Model_ObjetivoMetaPpa();
        $mppid = $_REQUEST['mppid'];
        $mdObjetivoMetappa->excluirPorObjetivo($oppid);
        foreach($mppid as $value){
            $mdObjetivoMetappa->salvarMetas($value, $oppid);
        }        
    }
    
    public function excluir($oppid)
    {
        $url = "?modulo=apoio/objetivoppa&acao=A";

        try {
            $mdObjetivoppa = new Public_Model_ObjetivoPpa($oppid);

            if($mdObjetivoppa->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mdObjetivoppa->oppid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mdObjetivoppa->oppstatus = 'I';
            $mdObjetivoppa->salvar();
            $mdObjetivoppa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdObjetivoppa->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
