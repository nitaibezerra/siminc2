<?php

require_once APPRAIZ . 'elabrev/www/_funcoes_termoCooperacao.php';

unset($_SESSION['elabrev']);

if ($_SESSION['usucpf']) {

    $sql = "select 
				* 
			from 
				elabrev.usuarioresponsabilidade ur
			inner join 
				seguranca.perfil pf on pf.pflcod = ur.pflcod and pflstatus = 'A' 
			where 
				usucpf = '{$_SESSION['usucpf']}' 
			and 
				rpustatus = 'A'";

    $rs = $db->carregar($sql);

    if ($rs) {
        foreach ($rs as $resp) {
            $tipo = verificaTipoPerfilConcedenteProponente($resp['pflcod']);
            if (!empty($resp['unicod'])) {
                if (is_array($_SESSION['elabrev']['unicod'])) {
                    if (!in_array($resp['unicod'], $_SESSION['elabrev']['unicod']))
                        $_SESSION['elabrev']['unicod'][] = $resp['unicod'];
                }else {
                    $_SESSION['elabrev']['unicod'][] = $resp['unicod'];
                }
                if (is_array($_SESSION['elabrev'][$tipo]['unicod'])) {
                    if (!in_array($resp['unicod'], $_SESSION['elabrev'][$tipo]['unicod']))
                        $_SESSION['elabrev'][$tipo]['unicod'][] = $resp['unicod'];
                }else {
                    $_SESSION['elabrev'][$tipo]['unicod'][] = $resp['unicod'];
                }
            }
            if (!empty($resp['prsano'])) {
                if (is_array($_SESSION['elabrev']['prsano'])) {
                    if (!in_array($resp['prsano'], $_SESSION['elabrev']['prsano']))
                        $_SESSION['elabrev']['prsano'][] = $resp['prsano'];
                }else {
                    $_SESSION['elabrev']['prsano'][] = $resp['prsano'];
                }
                if (is_array($_SESSION['elabrev'][$tipo]['prsano'])) {
                    if (!in_array($resp['prsano'], $_SESSION['elabrev'][$tipo]['prsano']))
                        $_SESSION['elabrev'][$tipo]['prsano'][] = $resp['prsano'];
                }else {
                    $_SESSION['elabrev'][$tipo]['prsano'][] = $resp['prsano'];
                }
            }
            if (!empty($resp['ungcod'])) {
                if (is_array($_SESSION['elabrev']['ungcod'])) {
                    if (!in_array($resp['ungcod'], $_SESSION['elabrev']['ungcod']))
                        $_SESSION['elabrev']['ungcod'][] = $resp['ungcod'];
                }else {
                    $_SESSION['elabrev']['ungcod'][] = $resp['ungcod'];
                }
                if (is_array($_SESSION['elabrev'][$tipo]['ungcod'])) {
                    if (!in_array($resp['ungcod'], $_SESSION['elabrev'][$tipo]['ungcod']))
                        $_SESSION['elabrev'][$tipo]['ungcod'][] = $resp['ungcod'];
                }else {
                    $_SESSION['elabrev'][$tipo]['ungcod'][] = $resp['ungcod'];
                }
            }
            if (!empty($resp['dircod'])) {
                if (is_array($_SESSION['elabrev']['dircod'])) {
                    if (!in_array($resp['dircod'], $_SESSION['elabrev']['dircod']))
                        $_SESSION['elabrev']['dircod'][] = $resp['dircod'];
                }else {
                    $_SESSION['elabrev']['dircod'][] = $resp['dircod'];
                }
                if (is_array($_SESSION['elabrev'][$tipo]['dircod'])) {
                    if (!in_array($resp['dircod'], $_SESSION['elabrev'][$tipo]['dircod']))
                        $_SESSION['elabrev'][$tipo]['dircod'][] = $resp['dircod'];
                }else {
                    $_SESSION['elabrev'][$tipo]['dircod'][] = $resp['dircod'];
                }
            }
            if (!empty($resp['cooid'])) {
                if (is_array($_SESSION['elabrev']['cooid'])) {
                    if (!in_array($resp['cooid'], $_SESSION['elabrev']['cooid']))
                        $_SESSION['elabrev']['cooid'][] = $resp['cooid'];
                }else {
                    $_SESSION['elabrev']['cooid'][] = $resp['cooid'];
                }
                if ($_SESSION['elabrev'][$tipo]['cooid']) {
                    if (!in_array($resp['cooid'], $_SESSION['elabrev'][$tipo]['cooid']))
                        $_SESSION['elabrev'][$tipo]['cooid'][] = $resp['cooid'];
                }else {
                    $_SESSION['elabrev'][$tipo]['cooid'][] = $resp['cooid'];
                }
            }
        }
    }
}

if ($_SESSION['elabrev']['unicod']) {
    $_SESSION['elabrev']['unicod'] = count($_SESSION['elabrev']['unicod']) == 1 ? $_SESSION['elabrev']['unicod'][0] : $_SESSION['elabrev']['unicod'];
    if ($_SESSION['elabrev']['concedente']['unicod'])
        $_SESSION['elabrev']['concedente']['unicod'] = count($_SESSION['elabrev']['concedente']['unicod']) == 1 ? $_SESSION['elabrev']['concedente']['unicod'][0] : $_SESSION['elabrev']['concedente']['unicod'];
    if ($_SESSION['elabrev']['proponente']['unicod'])
        $_SESSION['elabrev']['proponente']['unicod'] = count($_SESSION['elabrev']['proponente']['unicod']) == 1 ? $_SESSION['elabrev']['proponente']['unicod'][0] : $_SESSION['elabrev']['proponente']['unicod'];
    if ($_SESSION['elabrev']['prop_conc']['unicod'])
        $_SESSION['elabrev']['prop_conc']['unicod'] = count($_SESSION['elabrev']['prop_conc']['unicod']) == 1 ? $_SESSION['elabrev']['prop_conc']['unicod'][0] : $_SESSION['elabrev']['prop_conc']['unicod'];
}

if ($_SESSION['elabrev']['prsano']) {
    $_SESSION['elabrev']['prsano'] = count($_SESSION['elabrev']['prsano']) == 1 ? $_SESSION['elabrev']['prsano'][0] : $_SESSION['elabrev']['prsano'];
    if ($_SESSION['elabrev']['concedente']['prsano'])
        $_SESSION['elabrev']['concedente']['prsano'] = count($_SESSION['elabrev']['concedente']['prsano']) == 1 ? $_SESSION['elabrev']['concedente']['prsano'][0] : $_SESSION['elabrev']['concedente']['prsano'];
    if ($_SESSION['elabrev']['proponente']['prsano'])
        $_SESSION['elabrev']['proponente']['prsano'] = count($_SESSION['elabrev']['proponente']['prsano']) == 1 ? $_SESSION['elabrev']['proponente']['prsano'][0] : $_SESSION['elabrev']['proponente']['prsano'];
    if ($_SESSION['elabrev']['prop_conc']['prsano'])
        $_SESSION['elabrev']['prop_conc']['prsano'] = count($_SESSION['elabrev']['prop_conc']['prsano']) == 1 ? $_SESSION['elabrev']['prop_conc']['prsano'][0] : $_SESSION['elabrev']['prop_conc']['prsano'];
}

if ($_SESSION['elabrev']['ungcod']) {
    $_SESSION['elabrev']['ungcod'] = count($_SESSION['elabrev']['ungcod']) == 1 ? $_SESSION['elabrev']['ungcod'][0] : $_SESSION['elabrev']['ungcod'];
    if ($_SESSION['elabrev']['concedente']['ungcod'])
        $_SESSION['elabrev']['concedente']['ungcod'] = count($_SESSION['elabrev']['concedente']['ungcod']) == 1 ? $_SESSION['elabrev']['concedente']['ungcod'][0] : $_SESSION['elabrev']['concedente']['ungcod'];
    if ($_SESSION['elabrev']['proponente']['ungcod'])
        $_SESSION['elabrev']['proponente']['ungcod'] = count($_SESSION['elabrev']['proponente']['ungcod']) == 1 ? $_SESSION['elabrev']['proponente']['ungcod'][0] : $_SESSION['elabrev']['proponente']['ungcod'];
    if ($_SESSION['elabrev']['prop_conc']['ungcod'])
        $_SESSION['elabrev']['prop_conc']['ungcod'] = count($_SESSION['elabrev']['prop_conc']['ungcod']) == 1 ? $_SESSION['elabrev']['prop_conc']['ungcod'][0] : $_SESSION['elabrev']['prop_conc']['ungcod'];
}

if ($_SESSION['elabrev']['dircod']) {
    $_SESSION['elabrev']['dircod'] = count($_SESSION['elabrev']['dircod']) == 1 ? $_SESSION['elabrev']['dircod'][0] : $_SESSION['elabrev']['dircod'];
    if ($_SESSION['elabrev']['concedente']['dircod'])
        $_SESSION['elabrev']['concedente']['dircod'] = count($_SESSION['elabrev']['concedente']['dircod']) == 1 ? $_SESSION['elabrev']['concedente']['dircod'][0] : $_SESSION['elabrev']['concedente']['dircod'];
    if ($_SESSION['elabrev']['proponente']['dircod'])
        $_SESSION['elabrev']['proponente']['dircod'] = count($_SESSION['elabrev']['proponente']['dircod']) == 1 ? $_SESSION['elabrev']['proponente']['dircod'][0] : $_SESSION['elabrev']['proponente']['dircod'];
    if ($_SESSION['elabrev']['prop_conc']['dircod'])
        $_SESSION['elabrev']['prop_conc']['dircod'] = count($_SESSION['elabrev']['prop_conc']['dircod']) == 1 ? $_SESSION['elabrev']['prop_conc']['dircod'][0] : $_SESSION['elabrev']['prop_conc']['dircod'];
}

if ($_SESSION['elabrev']['cooid']) {
    $_SESSION['elabrev']['cooid'] = count($_SESSION['elabrev']['cooid']) == 1 ? $_SESSION['elabrev']['cooid'][0] : $_SESSION['elabrev']['cooid'];
    if ($_SESSION['elabrev']['concedente']['cooid'])
        $_SESSION['elabrev']['concedente']['cooid'] = count($_SESSION['elabrev']['concedente']['cooid']) == 1 ? $_SESSION['elabrev']['concedente']['cooid'][0] : $_SESSION['elabrev']['concedente']['cooid'];
    if ($_SESSION['elabrev']['proponente']['cooid'])
        $_SESSION['elabrev']['proponente']['cooid'] = count($_SESSION['elabrev']['proponente']['cooid']) == 1 ? $_SESSION['elabrev']['proponente']['cooid'][0] : $_SESSION['elabrev']['proponente']['cooid'];
    if ($_SESSION['elabrev']['prop_conc']['cooid'])
        $_SESSION['elabrev']['prop_conc']['cooid'] = count($_SESSION['elabrev']['prop_conc']['cooid']) == 1 ? $_SESSION['elabrev']['prop_conc']['cooid'][0] : $_SESSION['elabrev']['prop_conc']['cooid'];
}

// ver('','','',$_SESSION['elabrev']);
?>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>

    function montaShowModal() {
        var campoTextArea = '' +
                '<div>' +
                '<p align=center style=\'text-align:justify\'><b style=\'mso-bidi-font-weight:normal\'>O SIMEC estar� dispon�vel para a inclus�o de cr�ditos referentes ao segundo per�odo de 2011, de 18/08/2011 at� 31/08/2011.</b></p>' +
                '</div>';
        var alertaDisplay = '<div class="titulo_box" ><center><b>Prezados,</b></center><br/ >' + campoTextArea + '</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
        displayStaticMessage(alertaDisplay, false);
        return false;
    }

    function displayStaticMessage(messageContent, cssClass) {
        messageObj = new DHTML_modalMessage();	// We only create one object of this class
        messageObj.setShadowOffset(5);	// Large shadow

        messageObj.setHtmlContent(messageContent);
        messageObj.setSize(450, 140);
        messageObj.setCssClassMessageBox(cssClass);
        messageObj.setSource(false);	// no html source since we want to use a static message here.
        messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
        messageObj.display();
    }

    function closeMessage() {
        messageObj.close();
    }

</script>
<body>
    <div id="aviso" title="Aviso" style="display: none; padding: 20px">
<!--        De acordo com o art. 6� da Portaria n� 549, de 16 de abril de 2013, � 1�, o <i>�rg�o ou entidade recebedora dos recursos dever� encaminhar ao concedente relat�rio de cumprimento do objeto pactuado at� 60 dias do t�rmino do prazo para cumprimento do objeto estabelecido no Termo de Execu��o Descentralizada.
            Ainda no mesmo artigo, em seu � 3�, a celebra��o de novos termos de execu��o descentralizado com o �rg�o ou entidade recebedora dos recursos descentralizados ficar� condicionada a comprova��o de entrega do relat�rio de cumprimento do objeto, ap�s transcorrido o prazo estabelecido no par�grafo primeiro deste artigo.</i>
        <br/>
        <b><br/>Alertamos assim que aquelas institui��es que celebraram termos de execu��o descentralizado em 2013 no <?php echo SIGLA_SISTEMA; ?> devem realizar o preenchimento e entrega do relat�rio de cumprimento de objeto no prazo indicado, evitando que sejam bloqueados novos repasses conforme previsto no � 3� supracitado.
            <br/><br/>Ressaltamos ainda que o preenchimento do relat�rio tamb�m � feito via <?php echo SIGLA_SISTEMA; ?>, da mesma forma como � celebrado o termo de execu��o descentralizada. O relat�rio deve ser inserido pela Equipe T�cnica da entidade proponente, passando novamente pela valida��o do gestor or�ament�rio e do representante legal.
            Em seguida o termo chega � secretaria que analisou aquele termo, que poder� enviar para dilig�ncia ou emitir parecer e finalizar o termo.
        </b>-->
        
            Senhores Dirigentes,
            <br/><br/>
            Hoje, dia <b>26/03/2014</b>, � o prazo final para a solicita��o de altera��es or�ament�rias (cr�ditos) no primeiro momento.
            Lembramos que tal pedido deve ser efetuado no M�dulo de Altera��es Or�ament�rias. Caso a Unidade n�o possua o acesso, deve efetuar o pedido de acesso no Simec na primeira tela do Simec e entrar em contato por meio do e-mail <?php echo $_SESSION['email_sistema']; ?>.
            Ap�s a inser��o dos cr�ditos a unidade deve acessar a guia ?Resumo/Tr�mite? e clicar na op��o ?Enviar para SPO?. Essa a��o � necess�ria para que a SPO possa analisar as solicita��es e proceder o andamento do pedido.
<br/><br/>
            Estamos a disposi��o para esclarecimentos.
<br/><br/>
            Contatos:<br/>
            Universidades e Hospitais: (61) 2022-8860 / 8857 / 8855 / 8856
            <br/>Institutos: (61) 2022-8854 / 8858 / 8859 / 8861
<br/><br/>
            Luciane Tisbierek de Carvalho
            <br/>Coordenadora-Geral de Or�amento
        
    </div>
    <?php
    /**
     * Redireciona o navegador para a tela indicada.
     * 
     * @return void
     */
    $perfil = array(53, 54, 57, 55, 76, 388);

    function redirecionar($modulo, $acao, $parametros = array()) {
        $parametros = http_build_query((array) $parametros, '', '&');
        header("Location: ?modulo=$modulo&acao=$acao&$parametros");
        exit();
    }

    switch ($_REQUEST['evento']) {
        case 'cadastrar_anexo':
            $arquivo = $_FILES['arquivo'];
            if (!is_uploaded_file($arquivo['tmp_name']))
                $_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao enviar o arquivo, favor tentar novamente.";

            //caminho (nome do arquivo com data/hora.para nao sobrescrever.)
            $caminho = APPRAIZ . 'arquivos/elabrev/' . date("dmY-His") . $arquivo['name'];


            if (move_uploaded_file($arquivo['tmp_name'], $caminho)) {
                $sql = sprintf(
                        "INSERT INTO elabrev.documentos(
                                  taaid,
                                  usucpf,
                                  doctamanho,
                                  docnome,
                                  docdescricao,
                                  doccaminho,
                                  docmimetype
                                ) values (
                                   %d, 
                                  '%s',
                                   %d,
                                  '%s', 
                                  '%s',
                                  '%s',
                                  '%s'
                                ) returning docid", $_REQUEST['taaid'], $_SESSION['usucpf'], $arquivo['size'], $arquivo['name'], substr($_REQUEST['docdescricao'], 0, 250), basename($caminho), $arquivo['type']
                );

                if ($db->executar($sql)) {
                    $db->commit();
                } else {
                    $_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao cadastrar o arquivo no banco dedados.<br />Verifique as informa��es e tente novamente.";
                    $db->rollback();
                }
            } else {
                $_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao salvar o arquivo no servidor, favor tentar novamente.";
            }

            break;

        case 'excluir_anexo':
            $sql = "SELECT * FROM elabrev.documentos WHERE docstatus = 'A' AND docid = " . $_REQUEST['docid'];
            $arquivo = $db->pegaLinha($sql);

            $sql = sprintf("UPDATE elabrev.documentos SET docstatus = 'E' WHERE docid = %d", $_REQUEST['docid']);
            if (!$db->executar($sql)) {
                $_SESSION['MSG_AVISO'][] = "N�o foi poss�vel remover o documento.";
                $db->rollback();
            } else {
                // enquanto nao pode ser removido de verdade... fica coment�da a linha
                //if(unlink($this->arquivo_caminho))
                //{

                $db->commit();

                if (is_file(APPRAIZ . 'arquivos/elabrev/' . $arquivo['doccaminho'])) {
                    if (unlink(APPRAIZ . 'arquivos/elabrev/' . $arquivo['doccaminho'])) {
                        $_SESSION['MSG_AVISO'][] = "Documento exclu�do com �xito";
                    } else {
                        $_SESSION['MSG_AVISO'][] = "N�o foi poss�vel remover o documento.";
                    }
                } else {
                    $_SESSION['MSG_AVISO'][] = "N�o foi poss�vel remover o arquivo. Arquivo inexistente.";
                }
            }

            redirecionar($_REQUEST['modulo'], $_REQUEST['acao'], $parametros);
            break;

        case 'download':
            $sql = "SELECT * FROM elabrev.documentos WHERE docstatus = 'A' AND docid = " . $_REQUEST['arquivo'];
            $arquivo = $db->pegaLinha($sql);
            $caminho = APPRAIZ . 'arquivos/elabrev/' . $arquivo['doccaminho'];

            if (!is_file($caminho)) {
                $_SESSION['MSG_AVISO'][] = "Arquivo n�o encontrado.";
            }
            ob_clean();
            //tratando para abrir arquivos excel (antigos)
            if($arquivo['docmimetype'] == 'application/excel'){
            	header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
            } else {
            	header('Content-type: ' . $arquivo['docmimetype']);
            }
            //header('Content-type: ' . $arquivo['docmimetype']);
            header('Content-Disposition: attachment; filename=' . $arquivo['docnome']);
            readfile($caminho);
            exit();
            break;

        default:
            break;
    }


    /*
      if( possuiPerfil( $perfil ) && !($db->testa_superuser())){
      echo "<script>
      alert('A proposta or�ament�ria est� sendo transmitida para o SIOP e a partir do dia 3 de agosto estar� dispon�vel para visualiza��o!');
      </script>";
      } */


    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';
    monta_titulo(
            'Certificado de Disponibilidade Or�ament�ria - CDO', 'Notas T�cnicas Para Download'
    );
    ?>


    <script language="javascript" type="text/javascript">
        function cadastrar_anexo() {
            if (validar_formulario_anexo()) {
                document.anexo.submit();
            }
        }

        function validar_formulario_anexo() {
            var validacao = true;
            var mensagem = 'Os seguintes campos n�o foram preenchidos:';
            document.anexo.taaid.value = trim(document.anexo.taaid.value);
            document.anexo.docdescricao.value = trim(document.anexo.docdescricao.value);
            if (
                    document.anexo.taaid.value == '' ||
                    document.anexo.taaid.value == 0
                    ) {
                mensagem += '\nTipo';
                validacao = false;
            }
            if (document.anexo.docdescricao.value == '') {
                mensagem += '\nDescri��o';
                validacao = false;
            }
            if (document.anexo.arquivo.value == '') {
                mensagem += '\nArquivo';
                validacao = false;
            }
            if (!validacao) {
                alert(mensagem);
            }
            return validacao;
        }

        function excluir_anexo(anexo) {
            if (confirm('Deseja excluir o documento?')) {
                window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir_anexo&docid=' + anexo;
            }
        }


        function ltrim(value) {
            var re = /\s*((\S+\s*)*)/;
            return value.replace(re, "$1");
        }

        function rtrim(value) {
            var re = /((\s*\S+)*)\s*/;
            return value.replace(re, "$1");
        }

        function trim(value) {
            return ltrim(rtrim(value));
        }
    </script>
    <div style="border:solid red 1px;">
        Prezados usu�rios do <b>Termo de Execu��o Descentralizada</b>,
    <br/><br/>
    A partir do dia <b>06/05/2015</b> estar� no ar um <b>novo m�dulo</b> para o lan�amento dos Termos de Execu��o Descentralizada, contendo tamb�m todos os termos j� registrados.
    <br/>
    O <?php echo SIGLA_SISTEMA; ?> ir� redirecionar autom�ticamente o usu�rio ao clicar nos menus para esta finalidade.
    <br/>
    Todos os dados e perfis ser�o migrados. Caso encontre algum problema, entre em contato com a CGSO.
    <br/>
    <br/>
    Equipe <?php echo SIGLA_SISTEMA; ?>.
    </div>

    <table cellpadding="2" cellspacing="0" align="center" border="0">
        <tr>
            <td colspan="2">
                <?php if ($_SESSION['MSG_AVISO']): ?>
                    <font color="red">
                    <p>
                    <ul>
                        <li><?= implode('</li><li>', (array) $_SESSION['MSG_AVISO']); ?></li>
                    </ul>
                    </p>
                    </font>
                <? endif; ?>
                <?php $_SESSION['MSG_AVISO'] = array(); ?>
            </td>
        </tr>		
        <table>

            <table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
                <tr>
                    <td>
                        <!-- LISTA DE DOCUMENTOS -->
                        <?php
                        $sql = "SELECT 
              doc.docid,
              doc.doctamanho,
              doc.docnome,
              doc.docdescricao,
              to_char( doc.docdata,'dd/mm/YYYY') as docdata,
              usu.usunome,
              ta.taadescricao
        FROM elabrev.documentos AS doc
            INNER JOIN seguranca.usuario 	  AS usu ON (usu.usucpf = doc.usucpf)
            INNER JOIN pde.tipoanexoatividade AS ta  ON (ta.taaid   = doc.taaid)
        WHERE docstatus = 'A' 
        ORDER BY doc.docdata DESC
        ";
                        $documentos = $db->carregar($sql);
                        if (is_array($documentos)):
                            ?>
                            <table class='tabela' style="width:100%;" cellpadding="3">
                                <thead>
                                    <tr style="text-align: center; vertical-align: top">
                                        <th style="width: 5%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            A��o	
                                            </td>				

                                        <th style="width: 20%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Arquivo
                                        </th>
                                        <th style="width: 5%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Tamanho
                                        </th>
                                        <th style="width: 35%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Descri��o
                                        </th>
                                        <th style="width: 10%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Tipo
                                        </th>
                                        <th style="width: 10%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Data de Inclus�o
                                        </th>
                                        <th style="width: 20%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            Por
                                        </th>
                                    </tr>
                                </thead>				

                                <tbody>
                                    <?php
                                    /*
                                      echo "<pre>";
                                      print_r($documentos);
                                      Array
                                      (
                                      [0] => Array
                                      (
                                      [docid] => 1
                                      [taaid] => 1
                                      [usucpf] => 
                                      [doctamanho] => 1
                                      [docnome] => nota_concurso_cargos_mec_rfet.doc
                                      [docdescricao] => teste de nao sei o que lah das quantas....
                                      [docdata] => 2008-01-24 17:53:59.074084
                                      )

                                      )
                                     */
                                    foreach ($documentos as $documento):
                                        ?>
                                        <?php
                                        $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa';
                                        /*
                                          echo "<pre>";
                                          print_r($documento);
                                         */
                                        ?>

                                        <tr style=" background-color: <?= $cor ?>" >
                                            <td style="text-align:center" >
                                                <?php if ($db->testa_superuser() || $db->testa_cgo()): ?>
                                                    <img title="Remover o Documento" align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor = 'pointer'" onclick="excluir_anexo(<?= $documento['docid'] ?>);" title=""/>
                                                <?php else: ?>
                                                    &nbsp;
                                                <?php endif; ?>
                                            </td>				
                                            <td>
                                            <!-- <img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/> -->
                                                <a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=download&arquivo=<?= $documento['docid'] ?>">
                                                    <?= $documento['docnome'] ?>
                                                </a>
                                            </td>
                                            <td style="text-align: center"><?= round($documento['doctamanho'] / 1024) . " Kb" ?></td>
                                            <td style="text-align: left"><?= $documento['docdescricao'] ?></td>
                                            <td style="text-align: center"><?= $documento['taadescricao'] ?></td>
                                            <td style="text-align: center"><?= $documento['docdata'] ?></td>
                                            <td nowrap="nowrap"><?= $documento['usunome'] ?></td>				
                                        <?php endforeach; ?>
                                </tbody>
                            </table>
                        <?php else: ?>
                            <table class='tabela' style="width:100%;" cellpadding="3">
                                <tbody>
                                <td style="text-align:center;padding:15px;background-color:#f5f5f5;">
                                    Nenhum Documento Encontrado.
                                </td>
                                </tbody>
                            </table>
                        <?php endif; ?>

                    </td>
                </tr>
            </table>
            <div id="aviso" style="display: none;"><br>

                <p style="text-align: justify;">Senhor Dirigente,</p>

                <p style="text-align: justify;">Informamos que durante os dias <strong>03 e 04/ 08 </strong> 08 funcionar� um plant�o nesta SPO/MEC somente por meio do endere�o eletr�nico <?php echo $_SESSION['email_sistema']; ?>, com a finalidade de esclarecer d�vidas atinentes � inclus�o no <?php echo SIGLA_SISTEMA; ?> do detalhamento dos limites para o PLOA 2014. </p>

                <p style="text-align: justify;"><br>Atenciosamente,<br><br><strong>Wagner Vilas Boas de Souza<br>
                        Subsecret&aacute;rio de Planejamento e Or&ccedil;amento
                    </strong></p>
            </div>


            </body>
            <link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
            <script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
            <script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
            <script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
            <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

            <script language="javascript">
                                                        /* Alerta a pedido o Subsecret�rio Wagner */
                                                        //$("#aviso").dialog({position: 'top', width: 780, top: '20px'});

            </script>