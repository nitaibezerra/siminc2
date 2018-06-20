<?php

class Public_Controller_Indicadorpnc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/metapnc_form&acao=A&mpnid=' . $dados['mpnid'];

        try {
            $mMetapnc = new Public_Model_IndicadorPnc($dados['ipnid']);
            $mMetapnc->popularDadosObjeto($dados);
            $mMetapnc->prsano = $mMetapnc->prsano ? $mMetapnc->prsano : $_SESSION['exercicio'];
                
            $mMetapnc->salvar(null, null, ['ipndsc', 'ipnstatus', 'prsano', 'ipncod', ]);
            $mMetapnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($dados)
    {
        $url = '?modulo=apoio/metapnc_form&acao=A&mpnid=' . $dados['mpnid'];

        try {
            $mMetapnc = new Public_Model_IndicadorPnc($dados['mpnid']);
            
            if($mMetapnc->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/metapnc_form&acao=A&mpnid=' . $dados['mpnid'];
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mMetapnc->ipnstatus = 'I';
            $mMetapnc->salvar();
            $mMetapnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            