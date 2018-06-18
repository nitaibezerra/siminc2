<?php

class Public_Controller_Programa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/programa&acao=A';

        try {
            $mPrograma = new Monitora_Model_Programa($dados['prgid']);
            $mPrograma->popularDadosObjeto($dados);
            $mPrograma->prgano = $mPrograma->prgano ? $mPrograma->prgano : $_SESSION['exercicio'];
//            ver($mPrograma,d);

            $mPrograma->salvar(null,null );
            $mPrograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPrograma->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($prgid)
    {
        $url = '?modulo=apoio/programa&acao=A';

        try {
            $mPrograma = new Monitora_Model_Programa($prgid);
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
