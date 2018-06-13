<?php

class Public_Controller_Programa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/programa&acao=A';

        try {
            $mPrograma = new Monitora_Model_Programa($dados['prgid']);
            $mPrograma->popularDadosObjeto($dados);
            $mPrograma->mdeano = $mPrograma->mdeano ? $mPrograma->mdeano : $_SESSION['exercicio'];
//            ver($mPrograma,d);

            $mPrograma->salvar(null,null, ['prgid', 'prgcod', 'prgdscobjetivo', 'prgdsc', 'prgdscobjetivo', 'prgdscpublicoalvo']);
            $mPrograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPrograma->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($prgid)
    {
        $url = '?modulo=apoio/programa_form&acao=A';

        try {
            $mPrograma = new Monitora_Model_Programa($prgid);

/*            if($mPrograma->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/programa_form&acao=A&capid=' . $mPrograma->prgid;
                simec_redirecionar($url, 'error', $mensagem);
            }*/

            $mPrograma->prgstatus = 'I';
            $mPrograma->salvar();
            $mPrograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPrograma->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
