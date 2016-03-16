<?php

include_once '_funcoes_maismedicos.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class ManifestacaoVisitaInLoco {

    public $mntid;
    public $rqmid;
    public $mntdsc;
    public $usucpfmec;
    public $mntstatus;
    public $mntdtinclusaomec;
    /**/
    public $mrtid;
    public $mrtdsc;
    public $usucpfmunicipio;
    public $mrtstatus;
    public $mmrdata;

    public function salvarManifestacaoNotaTecnica($dadosPost, $db) {
        $erro = array();

        $this->mntid = $dadosPost['mntid'];
        $this->mntdtinclusaomec = date('Y-m-d');
        $this->mntdsc = addslashes($dadosPost['mntdsc']);
        $this->usucpfmec = $_SESSION['usucpf'];
        $this->mntstatus = 'A';

        $this->rqmid = (int) verificaExisteQuestionario();

        if (!is_uploaded_file($_FILES['arqid']['tmp_name'])) {
            $erro['msg'] = 'Arquivo é um campo obrigatório';
        } else if (empty($this->mntdsc)) {
            $erro['msg'] = 'Descrição é um campo obrigatório';
        }

        $arqid = $this->setIdArquivo(array('tabela' => 'manifestacaonotatecnica', 'esquema' => 'maismedicomec', 'descricao' => $this->mntdsc, 'campo' => 'arqid'));

        if (empty($erro)) {
            if ($this->mntid) {
                $sql = "UPDATE maismedicomec.manifestacaonotatecnica
                            SET  rqmid= {$this->rqmid}, mntdsc= '{$this->mntdsc}',
                            usucpfmec= '{$this->usucpfmec}', mntstatus= '{$this->mntstatus}', mntdtinclusaomec= '{$this->mntdtinclusaomec}'
                            WHERE mntid = {$this->mntid}
                            RETURNING mntid;
                                    ";
                $this->mntid = $db->pegaUm($sql);
                $sqlArquivo = "UPDATE maismedicomec.manifestacaonotatecnicaarquivo SET arqid =  {$arqid}  WHERE mntid = {$this->mntid} ;";
            } else {
                $sql = "INSERT INTO maismedicomec.manifestacaonotatecnica ( rqmid, mntdsc, usucpfmec, mntstatus, mntdtinclusaomec)
                               VALUES ({$this->rqmid}, '{$this->mntdsc}', '{$this->usucpfmec}', '{$this->mntstatus}', '{$this->mntdtinclusaomec}' ) RETURNING mntid;";
                $this->mntid = $db->pegaUm($sql);
                $sqlArquivo = "INSERT INTO maismedicomec.manifestacaonotatecnicaarquivo(mntid, arqid) VALUES ({$this->mntid}, {$arqid});";
            }

            if ($this->mntid) {
                $db->executar($sqlArquivo);
                $db->commit();
                echo
                '<script language="javascript">
                    alert("Operação efetuada com sucesso!");
                    location.href="par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A";
                </script>';
            } else {
                ver($erro, d);
            }
        } else {
            echo '<script>alert("' . $erro['msg'] . '");</script>';
        }
        return array($dadosPost, $erro);
    }

    public function salvarRespostaNotaTecnica($dadosPost, $db) {
        $erro = array();

        $this->mrtid = $dadosPost['mrtid'];
        $this->mntid = $dadosPost['mntid'];
        $this->mmrdata = date('Y-m-d');
        $this->mrtdsc = addslashes($dadosPost['mrtdsc']);
        $this->usucpfmunicipio = $_SESSION['usucpf'];
        $this->mrtstatus = 'A';

        if (!is_uploaded_file($_FILES['arqid']['tmp_name'])) {
            $erro['msg'] = 'Arquivo é um campo obrigatório';
        } else if (empty($this->mrtdsc)) {
            $erro['msg'] = 'Descrição é um campo obrigatório';
        }

        $arqid = $this->setIdArquivo(array('tabela' => 'manifrespostanotatecnica', 'esquema' => 'maismedicomec', 'descricao' => $this->mrtdsc, 'campo' => 'arqid'));

        if (empty($erro)) {
            if ($this->mrtid) {
                $sql = "UPDATE maismedicomec.manifrespostanotatecnica
                                        SET mntid = {$this->mntid},  mrtdsc= '{$this->mrtdsc}',
                                        usucpfmunicipio= '{$this->usucpfmunicipio}', mrtstatus= '{$this->mrtstatus}', mmrdata= '{$this->mmrdata}'
                                        WHERE mrtid = {$this->mrtid}
                                        RETURNING mntid;
                                    ";
                $this->mrtid = $db->pegaUm($sql);
                $sqlArquivo = "UPDATE maismedicomec.manifrespostanotatecnicaarquivo SET arqid =  {$arqid}  WHERE mrtid = {$this->mrtid} ;";
            } else {
                $sql = "INSERT INTO maismedicomec.manifrespostanotatecnica ( mntid, mrtdsc, usucpfmunicipio, mrtstatus, mmrdata)
                               VALUES ({$this->mntid}, '{$this->mrtdsc}', '{$this->usucpfmunicipio}', '{$this->mrtstatus}', '{$this->mmrdata}' ) RETURNING mrtid;";
                $this->mrtid = $db->pegaUm($sql);
                $sqlArquivo = "INSERT INTO maismedicomec.manifrespostanotatecnicaarquivo(mrtid, arqid) VALUES ({$this->mrtid}, {$arqid});";
            }

            if ($this->mrtid) {
                $db->executar($sqlArquivo);
                $db->commit();
                echo
                '<script language="javascript">
                    alert("Operação efetuada com sucesso!");
                    location.href="par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A";
                </script>';
            } else {
                ver($erro, d);
            }
        } else {
            echo '<script>alert("' . $erro['msg'] . '");</script>';
        }
        return array($dadosPost, $erro);
    }

    public function setIdArquivo(array $parans) {
        $file = new FilesSimec($parans['tabela'], null, $parans['esquema']);

        // restricao de numero de caracteres
            $descricaoLimitada = substr(addslashes($parans['descricao']), 0, 230);
            
        $file->setUpload( $descricaoLimitada, $parans['campo'], false, false);
        return (int) $file->getIdArquivo();
    }

    public function getSqlManifestacaoNotaTecnica() {
        $this->rqmid = (int) verificaExisteQuestionario();

        return "SELECT * 
                    FROM maismedicomec.manifestacaonotatecnica mnt
                    LEFT JOIN maismedicomec.manifestacaonotatecnicaarquivo mnta ON mnta.mntid =  mnt.mntid
                    JOIN seguranca.usuario usu ON usu.usucpf =  mnt.usucpfmec
                    WHERE rqmid = {$this->rqmid}
                        ; ";
    }

    public function getSqlManifestacaoRespostaNotaTecnica($mntid) {
        return "SELECT * FROM maismedicomec.manifrespostanotatecnica mrnt
                    LEFT JOIN maismedicomec.manifrespostanotatecnicaarquivo mrnta ON mrnta.mrtid = mrnt.mrtid 
                    JOIN maismedicomec.manifestacaonotatecnica mnt ON mnt.mntid = mrnt.mntid
                    JOIN seguranca.usuario usu ON usu.usucpf = mrnt.usucpfmunicipio
                    WHERE mrnt.mntid = {$mntid} ; ";
    }

    public function excluirManifestacaoNotaTecnica($id, $arqid, $db) {
        // $sqlArq = "DELETE FROM maismedicomec.manifestacaonotatecnicaarquivo WHERE mntid = {$id} ";
        // $db->executar($sqlArq);
        // $sql = "DELETE FROM maismedicomec.manifestacaonotatecnica WHERE mntid = {$id} ";
        $sql = "UPDATE maismedicomec.manifestacaonotatecnica SET mntstatus = 'I' WHERE mntid = {$id} ";
        $db->executar($sql);
        $db->commit();

        // $file = new FilesSimec(null, null, 'maismedicomec');
        // $file->excluiArquivoFisico($arqid);
        echo
        '<script language="javascript">
            alert("Dados excluídos com sucesso!");
            location.href="par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A";
        </script>';
    }

    public function excluirManifestacaoRespostaNotaTecnica($id, $arqid, $db) {
        // $sqlArq = "DELETE FROM maismedicomec.manifrespostanotatecnicaarquivo WHERE mrtid = {$id} ";
        // $db->executar($sqlArq);
        // $sql = "DELETE FROM maismedicomec.manifrespostanotatecnica WHERE mrtid = {$id} ";
        $sql = "UPDATE maismedicomec.manifrespostanotatecnica SET mrtstatus = 'I' WHERE mrtid = {$id} ";
        $db->executar($sql);

        $db->commit();

        // $file = new FilesSimec(null, null, 'maismedicomec');
        // $file->excluiArquivoFisico($arqid);
        echo
        '<script language="javascript">
            alert("Dados excluídos com sucesso!");
            history.back();
            //location.href="par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A";
        </script>';
    }

    public function getArquivo($idArquivo) {
        $file = new FilesSimec(null, null, 'maismedicomec');
        $file->getDownloadArquivo($idArquivo);
    }

}
