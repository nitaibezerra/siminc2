<?php

class Monitora_Controller_Ptres
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/ptres&acao=A';

        try {
            $mPtres = new Monitora_Model_Ptres($dados['ptrid']);
            $mPtres->popularDadosObjeto($dados);
            
            $mPtres->salvar(null, null, ['acaid', 'ptrano', 'funcod', 'sfucod', 'prgcod', 'acacod', 'loccod', 'unicod', 'irpcod', 'ptrdotacao', 'ptrstatus', 'ptrdata', 'plocod', 'esfcod', 'ptrdotacaocapital', 'ptrdotacaocusteio', 'plodsc', ]);
            $mPtres->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPtres->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($ptrid)
    {
        $url = '?modulo=apoio/ptres&acao=A';

        try {
            $mPtres = new Monitora_Model_Ptres($ptrid);
            
            if($mPtres->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mPtres->ptrid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mPtres->ptrstatus = 'I';
            $mPtres->salvar();
            $mPtres->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPtres->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            