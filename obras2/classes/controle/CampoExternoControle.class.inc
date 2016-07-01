<?php

include_once APPRAIZ . "includes/workflow.php";

class CampoExternoControle {

    private $htm;
    private static $camposDefault;

    //Função obrigatória. Nessa função deverá ser montado a questão que aparecerá dentro do campo externo. Deverá ser colocada dentro do $this->htm.
    public function montaNovoCampo($perid, $qrpid, $percent = 90) {
               
        $obQestionario = new QQuestionarioResposta();
		$queid = $obQestionario->pegaQuestionario( $qrpid );
        global $db;
        if($queid == QUEID_QUEST_CHKLST_CUMPRIMENTO) {
            $obQuest = new QuestoesCumprimentoObjetoAnexos();

            $dadosResp = $obQuest->pegaResposta($qrpid, $perid);
            $itpid = $db->pegaUm("SELECT itpid FROM questionario.pergunta WHERE perid = ".$perid);

            $htm .= "<input type='hidden' name='salvar_questionario' id='salvar_questionario' value='1'>";
            $htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
            $htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
            $htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
            $htm .= "<input type='hidden' name='arqid[{$perid}]' id='arqid' value='".($dadosResp['arqid'] ? 1 : 0 )."'>";

            if ($dadosResp['arqid']){
                #Adicionar regras no futuro.
                if (true) {
                    $boExcluir = "<a href=\"#\" onclick=\"excluirArquivo( ".$dadosResp['arqid'].", ".$perid." );\" title=\"Excluir Anexo\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
                } else {
                    $boExcluir = "<a href=\"#\" title=\"Excluir Anexo\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
                }
                $htm .= $boExcluir." Arquivo em anexo: <a onclick=\"janela('?modulo=principal/popupQuestionarioCumprimento&acao=A&requisicao=download&arqid={$dadosResp['arqid']}', 600, 480);\">".$dadosResp['arqdescricao']."</a>";
            } else {
                $htm .= "<input type='file' name='{$perid}' id='arquivo_itpid_{$itpid}' />";
            }
            $this->htm = $htm;
            return;
        }
        $htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
        $htm .= "<input type='hidden' name='salvar_questionario' id='salvar_questionario' value='1'>";
        $htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
        $htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";

     //   self::$camposDefault = true;
        
        $sql = "SELECT
						*
					FROM
						obras2.questoesanexascumprimento a
					INNER JOIN
						public.arquivo ar on ar.arqid = a.arqid
					WHERE
						a.qprid = {$qrpid} AND
						a.perid ={$perid}";
        $dados = $db->pegaLinha($sql);
        
        if (empty($dados['arqid'])) {
            $htm .= "<input name='arquivo' type='file'>";
        }else{
            
            
             $server = $_SERVER['SERVER_NAME']; 
            $endereco = $_SERVER ['REQUEST_URI'];
            $url = "http://" . $server . $endereco;
            
            $htm = "<a href='$url&arquivo_download={$dados['arqid']}'>Arquivo pra download</a>";
            $htm .= ' <a style="color:red" href="'.$url.'&deletar_arquivo='.$dados['arqid'].'">Excluir arquivo</a>';
            
            
        }

        $this->htm = $htm;
    }

    //Da echo no resultado do campo externo para alimentar o relatorio de respostas do questionario.
    //Sem essa função o relatório apenas exibirá o texto "Campo Externo".
    function retornoRelatorio($qrpid, $perid) {
        
        global $db;


        $sql = "SELECT
						*
					FROM
						obras2.questoesanexascumprimento a
					INNER JOIN
						public.arquivo ar on ar.arqid = a.arqid
					WHERE
						a.qprid = {$qrpid} AND
						a.perid ={$perid}";
        $dados = $db->pegaLinha($sql);

        if (!empty($dados['arqid'])) {
            
            $server = $_SERVER['SERVER_NAME']; 
            $endereco = $_SERVER ['REQUEST_URI'];
            $url = "http://" . $server . $endereco;
            
            echo "<a href='$url&arquivo_download={$dados['arqid']}'>Arquivo pra download</a>";
        } else {
            echo "<span style='color:#F00'>É necessário inserir o arquivo nesse campo</span>";
        }
    }

    //Função que salva o resultado do campo externo. Não deverá ser salvo no esquema do questionário, e sim no esquema do próprio sistema.
    function salvar() {
        if(!$_POST['arqid']) {
            return;
        }
        foreach($_POST['arqid'] as $perid => $val) {
            if(!$_FILES[$perid]) continue;
            unset($_POST['perg'][$perid]);
            if($_FILES[$perid]['error'] == '0') {
                $campos = array('qrpid' => $_POST['qrpid'], 'perid' => $perid);
                $file = new FilesSimec("questoes_cumprimentoobjeto_anexos", $campos, 'obras2');
                if (is_file($_FILES[$perid]["tmp_name"])) {
                    $arquivoSalvo = $file->setUpload($_FILES[$perid]["name"], $perid);
                    $_POST['perg'][$perid] = '1';
                }
            }
        }
    }

    //Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
    function show() {
        echo $this->htm;
    }

}
