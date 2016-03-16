<?php
class Spo_Service_Espelho extends Spo_Service_Abstract
{
    private $modelAnexoGeral;

    public function __construct()
    {
        ;
    }

    public function buscaAnexo($arptipo,$angtipoanexo = 'E')
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        $this->modelAnexoGeral->__set('arptipo',$arptipo);
        $this->modelAnexoGeral->__set('angtipoanexo',$angtipoanexo);
        return $this->modelAnexoGeral->recuperaAnexoGeral();
    }

    public function buscaAnexoPorId($arqid)
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        $this->modelAnexoGeral->__set('arqid',$arqid);
        return $this->modelAnexoGeral->buscaAnexo();
    }

    public function cadastroAnexo($dados)
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        if($_FILES["file"][size] > 0) {
            if ($this->verificaExtensao($_FILES['file']['name'], array('pdf'))) {
                $this->getFlashMessage()->addMensagem('Por favor, envie arquivos com a extensão PDF.',  Simec_Helper_FlashMessage::AVISO);
                return false;
            } 
            $campos = array ("angdsc" => "'".$dados['nomeArquivo']."'","arptipo" => "'".$dados['tipo']."'","arpdtinclusao" => "'now()'","angtipoanexo" => "'E'","arpstatus" => "'A'", "exercicio" => $_SESSION['exercicio']);
            $file = new FilesSimec("anexogeral", $campos, "spo");
            $arquivoSalvo = $file->setUpload($_FILES['file']['name'], '',true);
            if ($arquivoSalvo) {
                return true;
            }
            $this->disparoAvisoPosExecucao();
        }
    }

    public function atualizaAnexo($dados)
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        $this->modelAnexoGeral->__set('angdsc',$dados['nomeArquivo']);
        $this->modelAnexoGeral->__set('arptipo',$dados['tipo']);
        $this->modelAnexoGeral->__set('arqid',$dados['arqid']);
        return $this->modelAnexoGeral->alteraAnexo();
   }

   public function removeAnexo($arqid)
   {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        $this->modelAnexoGeral->excluir($arqid);
        return $this->modelAnexoGeral->commit();
   }

    public function retornaExtensao($arquivo)
    {
        return strtolower(end(explode('.',$arquivo)));
    }

    public function verificaExtensao($arquivo, $extensao)
    {
        return array_search($this->retornaExtensao($arquivo), $extensao) === false;
    }

    public function disparoAvisoPosExecucao()
    {
        if(is_array( $_FILES ["file"]) && $_FILES['file']['error'] > 0){
            switch ($_FILES['file']['error'] ) {
                case UPLOAD_ERR_INI_SIZE:
                    $response = 'O arquivo enviado excede o tamanho máximo de 2M.';
                    break;
                case UPLOAD_ERR_FORM_SIZE:
                    $response = 'O arquivo enviado excede o tamanho de 2M, escolha outro arquivo respeitando o tamanho permitido';
                    break;
                case UPLOAD_ERR_NO_FILE:
                    $response = 'Nenhum arquivo foi enviado';
                    break;
                case UPLOAD_ERR_CANT_WRITE:
                    $response = 'Falha ao gravar o arquivo no disco. Tente novamente.';
                    break;
                default:
                    $response = 'Ocorreu um erro ao enviar, tente novamente.';
                    break;
            }
            $this->getFlashMessage()->addMensagem($response,  Simec_Helper_FlashMessage::AVISO);
        }
    }

    public function downloadArquivo($arqid)
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        $this->modelAnexoGeral->__set('arqid',$arqid);
		$arquivo = $this->modelAnexoGeral->buscaDadosArquivo();
		if(!$arquivo){
			$this->getFlashMessage()->addMensagem("Arquivo não encontrado.", Simec_Helper_FlashMessage::ERRO);
			$this->fechaRecarrega("Arquivo não encontrado.");
            exit;
		}
		if(!is_file($this->getCaminho($arqid))){
            $this->getFlashMessage()->addMensagem("Arquivo inexistente na pasta.",  Simec_Helper_FlashMessage::ERRO);
            $this->fechaRecarrega("Arquivo inexistente na pasta.");
            exit;
		}
        $this->download($arquivo['arqnome'], $arquivo['arqextensao'], $arquivo['arqtipo']);
        readfile($this->getCaminho($arqid));
        exit();
	}

    private function getCaminho($arqid)
    {
        return APPRAIZ."arquivos/spo/".floor($arqid/1000).'/'.$arqid;
    }

    private function download($arqnome, $arqextensao, $arqtipo)
    {
        $filename = str_replace(" ", "_", $arqnome.'.'.$arqextensao);
	    header('Content-type: '. $arqtipo);
        header('Content-Disposition: attachment; filename='.$filename);
    }

    private function fechaRecarrega($mensagem)
    {
        if($mensagem){
            $console = "alert('{$mensagem}')";
        }
        echo <<<HTML
            <script>
                {$console}
                window.close();
                window.opener.location.reload();                
            </script>
HTML;
    }

    public function permiteAcesso()
    {
        $this->modelAnexoGeral = new Spo_Model_AnexoGeral();
        return $this->modelAnexoGeral->permissaoAcesso();
    }

}
