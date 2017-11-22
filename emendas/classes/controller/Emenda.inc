<?php

class Emendas_Controller_Emenda
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/emenda_form&acao=A';

        try {
            $mEmenda = new Emendas_Model_Emenda($dados['emeid']);
            $mEmenda->popularDadosObjeto($dados);
            $mEmenda->prsano = $mEmenda->prsano ? $mEmenda->prsano : $_SESSION['exercicio'];
                
            $mEmenda->salvar(null, null, ['emejustificativa', 'emeimpositiva', ]);
            $mEmenda->commit();
            simec_redirecionar($url . '&emeid=' . $mEmenda->emeid, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($emeid)
    {
        $url = '?modulo=principal/emenda&acao=A';

        try {
            $mEmenda = new Emendas_Model_Emenda($emeid);
            
            if($mEmenda->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mEmenda->emeid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mEmenda->emestatus = 'I';
            $mEmenda->salvar();
            $mEmenda->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function importar($files)
    {
        if($files['error'] == UPLOAD_ERR_OK){
            $extensao = substr($files['name'], strrpos($files['name'], '.'));

            $origem = $files['tmp_name'];
            $destino = APPRAIZ . 'emendas/modulos/apoio/importacao/planilha_' . date('YmdHis') . $extensao;
            if(move_uploaded_file($origem , $destino)){

                $file = fopen($destino, 'r');

                $count = 0;
                while (($line = fgetcsv($file, 0, ';')) !== false)
                {

                    $line = array_map('trim', $line);
//                    $line = array_map('utf8_decode', $line);

                    $mEmendas = new Emendas_Model_Emenda();
                    if(!$count++){ continue; }

                    // Unidade Orçamentária
                    $unocod = substr(trim($line[5]), 0, 5);
                    $sql = "select unoid from public.unidadeorcamentaria where unocod = '{$unocod}' and prsano = '{$_SESSION['exercicio']}'";
                    $unoid = $mEmendas->pegaUm($sql);

                    // Autor
                    $autor = explode('-', $line[0]);
                    $autcod = trim($autor[0]);
                    $sql = "select autid from emendas.autor where autcod = '{$autcod}' order by autstatus";
                    $autid = $mEmendas->pegaUm($sql);

                    // Ação
                    $acacod = substr(trim($line[7]), 0, 4);
                    $prgcod = substr(trim($line[6]), 0, 4);
                    $loccod = substr(trim($line[8]), 0, 4);
                    $sql = "select acaid from monitora.acao where acacod = '$acacod' and prgcod = '$prgcod' and loccod = '$loccod' and unicod = '$unocod' and prgano = '{$_SESSION['exercicio']}'";

                    $acoes = $mEmendas->carregar($sql);

                    if(!$acoes || !$autid){
                        continue;
                    }

                    if(count($acoes)> 1){
                        ver('Mais de uma ação', d);
                    }

                    $mEmendas->emenumero = trim($line[2]);
                    $mEmendas->unoid = $unoid;
                    $mEmendas->acaid = $acoes[0]['acaid'];
                    $mEmendas->autid = $autid;
                    $mEmendas->prsano = $_SESSION['exercicio'];

                    try{
                        $mEmendas->salvar();
                        $mEmendas->commit();
                        unset($mEmendas);
                    } catch (Exception $e){

                    }
                }
                fclose($file);
            }
        }
        echo 'Fim';
    }

    public function montarFormularioDetalhes($emeid = null, $emdid = null)
    {
        global $simec;
        $mEmendadetalhe = new Emendas_Model_EmendaDetalhe($emdid);

        if(!$mEmendadetalhe->emeid){ $mEmendadetalhe->emeid = $emeid; }
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe_form.inc';
    }

    public function montarListagemDetalhes($emeid)
    {
        $aEmendadetalhe = (new Emendas_Model_Emendadetalhe)->recuperarTodos('*', ['emeid = ' . (int)$emeid]);
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe.inc';
    }

}            