<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Proporc_Service_Prelimites
{
    private $acceptedExtension = array('xls', 'xlsx');

    public function cadastrar($dados)
    {
        $resultado = array('msg' => 'Não foram inseridos arquivos para upload.', 'sucesso' => false);
        //Dados do pré-limite atual a serem atualizados.
        $dadosUpdate = array('prpid' => $dados['prpid'], 'dataultimaatualizacao' => 'now()');
        if($this->verificaPerfil()){
            $dadosUpdate['usucpf'] = $_SESSION['usucpf'];
        }

        $prelimite = new Proporc_Model_PrelimitesPessoal();
        $prelimite->__set('prpid',$dados['prpid']);
        $docid = $prelimite->verificaDocid();
        if(!$docid){
            $docid = wf_cadastrarDocumento(TPDOC_PRELIMITES_PESSOAL, 'Prelimites_pessoal');
            $dadosUpdate['docid'] = $docid;
        }

        //Atualizando pré-limite.
        $prelimite->popularDadosObjeto($dadosUpdate);
        if($prelimite->alterar()){
            $prelimite->commit();
            $resultado['msg'] = 'Pré-Limites atualizado com sucesso!';
            $resultado['sucesso'] = true;
        }

        //Carregando arqid para o Pré-Limites Pessoal atual.
        $arqidM = $prelimite->carregaArqid($dados['prpid'],'M',$_SESSION['exercicio']);
        $arqidP = $prelimite->carregaArqid($dados['prpid'],'P',$_SESSION['exercicio']);

        //Upload do modelo.
        if ($_FILES['modelo']['size'] && $_FILES['modelo']['error']=='0') {
            $fileinfo = pathinfo($_FILES['modelo']['name']);
            if (in_array(strtolower($fileinfo['extension']), $this->acceptedExtension)) {
                $this->removeUpload($arqidM);
                $modelo = $this->uploadModelo($dados['prpid'],$dados['unicod']);
            }else{
                $resultado['msg'] .= '<br>Arquivo <b>Modelo</b> com formato inválido.';
                $resultado['sucesso'] = false;
                $resultado['warning'] = true;
            }
        }

        //Upload do modelo preenchido.
        if ($_FILES['preenchimento']['size'] && $_FILES['preenchimento']['error']=='0') {
            $fileinfo = pathinfo($_FILES['preenchimento']['name']);
            if (in_array(strtolower($fileinfo['extension']), $this->acceptedExtension)) {
                $this->removeUpload($arqidP);
                $preenchimento = $this->uploadPreechimento($dados['prpid'],$dados['unicod']);
            }else{
                $resultado['msg'] .= '<br>Arquivo <b>Modelo Preenchido</b> com formato inválido.';
                $resultado['sucesso'] = false;
                $resultado['warning'] = true;
            }
        }

        if($modelo){
            $resultado['msg'] .= '<br>Arquivo <b>Modelo</b> inserido com sucesso!';
            $resultado['sucesso'] = true;
        }
        if($preenchimento){
            $resultado['msg'] .= '<br>Arquivo <b>Modelo Preenchido</b> inserido com sucesso!';
            $resultado['sucesso'] = true;
        }
        return $resultado;
    }

    public function verificaPerfil()
    {
        if(in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))){
            return true;
        }
        return false;
    }


    public function uploadModelo($prpid,$unicod)
    {
        $campos = array(
            'angdsc' => "'{$unicod}_modelo'",
            'angtip' => "'PP'",
            'angtipoanexo' => "'L'",
            'angano' => $_SESSION['exercicio'],
            'prpid' => $prpid,
            'tipo' => "'M'"
        );
        $file = new FilesSimec('anexogeral', $campos ,'proporc');
        return $file->setUpload($unicod.'_modelo', '', true);
    }

    public function uploadPreechimento($prpid,$unicod)
    {
        $campos = array(
            'angdsc' => "'{$unicod}_preenchido'",
            'angtip' => "'PP'",
            'angtipoanexo' => "'L'",
            'angano' => $_SESSION['exercicio'],
            'prpid' => $prpid,
            'tipo' => "'P'"
        );

        $file = new FilesSimec('anexogeral', $campos ,'proporc');
        return $file->setUpload($unicod.'_preenchimento', 'preenchimento');
    }

    public function removeUpload($arqid = null)
    {
        if($arqid == null){
            return;
        }
        $prelimites = new Proporc_Model_PrelimitesPessoal();
        if($prelimites->deletaArqid($arqid)){
            $file = new FilesSimec('anexogeral', null ,'proporc');
            $file->setPulaTableEschema(true);
            $file->setRemoveUpload($arqid);
        }

    }
    /**
    * Pega o docid de um prelimite
    * @param int $prpid
    */
    public function pegaDocid($prpid)
    {
       global $db;
       $strSQL = "select docid from proporc.prelimites_pessoal where prpid = %d";
       return $db->pegaUm(sprintf($strSQL, (int) $prpid));
    }

    /**
    * Pega o estado atual do workflow
    * @param integer $prpid
    * @return integer
    */
    public function pegarEstadoAtual($prpid)
    {
       global $db;

       $docid = $this->pegaDocid($prpid);
       if ($docid) {
           $strSQL = sprintf("SELECT ed.esdid
                   FROM workflow.documento d
                       JOIN workflow.estadodocumento ed ON(ed.esdid = d.esdid)
                   WHERE d.docid = %d", (int) $docid);
           $estado = (integer) $db->pegaUm($strSQL);
           return $estado;
       }

       return false;
    }

    /**
    * @param string $unicod
    * @return string
    */
    public function pegarUO($unicod)
    {
        global $db;

        $strSQL = sprintf("
            select uni.unicod|| ' - '|| uni.unidsc as unidade from public.unidade uni where unicod = '%s'
        ", $unicod);
        return (string) $db->pegaUm($strSQL);
    }

    /**
    * Pegar o ID do perfil atual
    * @param string $usucpf
    * @return integer|boolean
    */
    public function pegarPerfilAtual($usucpf)
    {
        global $db;

        $sql = "select ur.pflcod from proporc.usuarioresponsabilidade ur where ur.usucpf = '%s'";
        $strSQL = sprintf($sql, (string) $usucpf);
        $pflcod = (integer) $db->pegaUm($strSQL);

        if (!$pflcod) {
            $sql = "SELECT u.pflcod FROM seguranca.perfilusuario u WHERE u.usucpf = '%s' and u.pflcod in (%d, %d)";
            $strSQL = sprintf($sql, (string) $usucpf, PFL_ADMINISTRADOR, PFL_CGO_EQUIPE_ORCAMENTARIA);
            $pflcod = (integer) $db->pegaUm($strSQL);
        }

        return ($pflcod) ? $pflcod : FALSE;
    }

    public function pegaPrpidPorUnidade($unicod)
    {
        $prelimite = new Proporc_Model_PrelimitesPessoal();
        $prelimite->__set('unicod', $unicod);
        return $prelimite->pegaId();
    }

    public function capturaDados($id)
    {
        $prelimite = new Proporc_Model_PrelimitesPessoal();
        return $prelimite->carregarPorId($id);
    }

    public function recuperaModelo($prpid, $baixar = false)
    {
        global $db;
        $sql = "select arqid from proporc.anexogeral where prpid = $prpid and tipo = 'M'";
        $arqid = $db->pegaUm($sql);
        if(!$arqid){
            return false;
        }
        $file = new FilesSimec('anexogeral', $campos ,'proporc');
        if($baixar){
            $file->getDownloadArquivo($arqid);
            return;
        }
        return $file->getArquivo($arqid);
    }

    public function recuperaModeloPreenchido($prpid, $baixar = false)
    {
        global $db;
        $sql = "select arqid from proporc.anexogeral where prpid = $prpid and tipo = 'P'";
        $arqid = $db->pegaUm($sql);
        if(!$arqid){
            return false;
        }
        $file = new FilesSimec('anexogeral', $campos ,'proporc');
        if($baixar){
            $file->getDownloadArquivo($arqid);
            return;
        }
        return $file->getArquivo($arqid);
    }

    public function modalAltUsuario($prpid, $usucpf)
    {
        $sql = <<<DML
            SELECT usu.usucpf AS codigo,
                usu.usucpf || ' - ' || usu.usunome AS descricao
            FROM seguranca.perfilusuario pfu
            LEFT JOIN seguranca.usuario usu USING(usucpf)
            WHERE pfu.pflcod = %d
DML;
        $stmt = sprintf($sql, PFL_CGO_EQUIPE_ORCAMENTARIA);
        $combo = inputCombo('usucpf', $stmt, $usucpf, 'usucpf',array('return'=>true));

        echo <<<HTML
        <div class="col-md-12">
            <form class="form-horizontal" id="formAltUsuario" method="POST" role="form">
                <input type="hidden" name="prpid" value="{$prpid}" />
                <input type="hidden" name="requisicao" value="alterarUsuarioResponsavel" />
                <div class="form-group row">
                    <label class="control-label col-md-2" for="usucpf">Responsável: </label>
                    <div class="col-md-10">
                        {$combo}
                    </div>
                </div>
            </form>
        </div>
        <script type="text/javascript" lang="JavaScript">
            $('#usucpf').chosen();
            $('#usucpf_chosen').css('width', '100%');
        </script>
HTML;
    }

    public function atualizarResponsavel($prpid, $usucpf)
    {
        $prelimite = new Proporc_Model_PrelimitesPessoal();
        $prelimite->__set('prpid',$prpid);
        $prelimite->__set('usucpfresponsavel',$usucpf);

        return $prelimite->alterarResponsavel($dados);
    }

}