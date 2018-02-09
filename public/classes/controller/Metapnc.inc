<?php

class Public_Controller_Metapnc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/metapnc&acao=A';

        try {
            $mMetapnc = new Public_Model_MetaPnc($dados['mpnid']);
            $mMetapnc->popularDadosObjeto($dados);
            $mMetapnc->prsano = $mMetapnc->prsano ? $mMetapnc->prsano : $_SESSION['exercicio'];
                
            $mMetapnc->salvar(null, null, ['mpndsc', 'mpnstatus', 'prsano', ]);
            $mMetapnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($mpnid)
    {
        $url = '?modulo=apoio/metapnc&acao=A';

        try {
            $mMetapnc = new Public_Model_MetaPnc($mpnid);
            
            if($mMetapnc->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mMetapnc->mpnid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mMetapnc->mpnstatus = 'I';
            $mMetapnc->salvar();
            $mMetapnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            