<?php

class Emendas_Controller_Proponentetipo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/proponentetipo&acao=A';

        try {
            $mProponentetipo = new Emendas_Model_Proponentetipo($dados['prtid']);
            $mProponentetipo->popularDadosObjeto($dados);
            
            $mProponentetipo->salvar(null, null, []);
            $mProponentetipo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProponentetipo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($prtid)
    {
        $url = '?modulo=apoio/proponentetipo&acao=A';

        try {
            $mProponentetipo = new Emendas_Model_Proponentetipo($prtid);
            
            if($mProponentetipo->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mProponentetipo->prtid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mProponentetipo->prtstatus = 'I';
            $mProponentetipo->salvar();
            $mProponentetipo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProponentetipo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            