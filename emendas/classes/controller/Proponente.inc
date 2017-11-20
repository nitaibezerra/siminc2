<?php

class Emendas_Controller_Proponente
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/proponente&acao=A';

        try {
            $mProponente = new Emendas_Model_Proponente($dados['proid']);
            $mProponente->popularDadosObjeto($dados);
            
            $mProponente->salvar(null, null, ['protelefone', 'proemail', ]);
            $mProponente->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProponente->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($proid)
    {
        $url = '?modulo=apoio/proponente&acao=A';

        try {
            $mProponente = new Emendas_Model_Proponente($proid);
            
            if($mProponente->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mProponente->proid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mProponente->prostatus = 'I';
            $mProponente->salvar();
            $mProponente->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProponente->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            