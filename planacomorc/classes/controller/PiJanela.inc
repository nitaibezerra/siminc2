<?php

class Planacomorc_Controller_PiJanela
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/pi_janela&acao=A';

        try {
            $mPiJanela = new Planacomorc_Model_PiJanela($dados['pijid']);
            $mPiJanela->popularDadosObjeto($dados);
            $mPiJanela->prsano = $mPiJanela->prsano ? $mPiJanela->prsano : $_SESSION['exercicio'];
                
            $mPiJanela->salvar(null, null, []);
            $mPiJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPiJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($pijid)
    {
        $url = '?modulo=apoio/pi_janela&acao=A';

        try {
            $mPiJanela = new Planacomorc_Model_PiJanela($pijid);
            
            if($mPiJanela->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mPiJanela->pijid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mPiJanela->pijstatus = 'I';
            $mPiJanela->salvar();
            $mPiJanela->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPiJanela->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            