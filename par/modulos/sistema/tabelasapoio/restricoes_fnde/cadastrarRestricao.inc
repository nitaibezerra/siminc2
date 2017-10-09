<?php
require_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

#armazena valores da requisicao
$requisicao = $_REQUEST['requisicao'];
$resid = $_REQUEST['resid'];

# Escolhe a acao a ser tomada pelo sistema
switch ($requisicao) {
    case 'salvar':
        $_REQUEST['usucpf'] = $_SESSION['usucpf'];
        $resid = salvarRestricaoFNDE($_REQUEST);
        $script = "
                <script>
                    alert('Restrição salva com sucesso!');
                    window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&resid=" . $resid . "';
                </script>
            ";
        echo $script;
        die;
        break;
    case 'anexarArquivo':
        anexarArquivo();
        die;
        break;
    case 'baixarArquivo':
        baixarArquivo();
        die;
        break;
    case 'excluirArquivo':
        excluirArquivo();
        die;
        break;
}

$listaMenus = buscarAbasRestricoes();
echo montarAbasArray($listaMenus, '/par/par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A');

/**
 * Salva os dados da restricao do FNDE
 * 
 * @global type $db
 * @param array $post
 * @return VOID
 */
function salvarRestricaoFNDE(array $post) {
    global $db;
    try {
        #Objeto Restricao FNDE
        $restricaoFnde = new RestricaoFnde();
        if (empty($post['resid'])) {
            #Inserir restricao
            $resid = $restricaoFnde->insert($post);
            if (!empty($resid)) {
                $restricaoEntidade = new RestricaoEntidade();
                $restricaoEntidade->inserirRestricaoEntidade($post, $resid);
            }
        } else {
            #alterar restricao
            $resid = $post['resid'];
            $restricaoFnde->update($post, $resid);
            
            #restricao entidade
            $restricaoEntidade = new RestricaoEntidade();
            $restricaoEntidade->atualizarRestricaoEntidade($post, $resid);
        }

        # Enviar email para o gestor do Projeto
        if ($post['resinformar'] == 'S') {
            enviarEmailRestricaoMunicipio($post);
        }

        # Anexo de arquivos
        $campos = array('resid' => $resid);
        $file = new FilesSimec("anexorestricao", $campos, 'par');

        if ($_FILES["arquivo"]['name'] !== '') {
            $file->setUpload($_FILES["arquivo"]['name']);
        }
        $db->commit();
        return $resid;
    } catch (Exception $exc) {
        $db->rollback();
        echo $exc->getTraceAsString();
    }
}

function enviarEmailRestricaoMunicipio($post) {
    # Retorna Lista de E-mail por Municipio
    $intrumentoUnidade = new InstrumentoUnidade();
    $arrEmail = $intrumentoUnidade->getMailByMunicipioUf($post);

    # Popula TipoRestricaoFnde
    $tipoRestricaoFnde = new TipoRestricaoFnde();
    $arrTipoRestricaoFnde = $tipoRestricaoFnde->findByPK($post['tprid']);

    #@TODO remover linha abaixo para mandar e-mail municipio/estado
    foreach ($arrEmail as $email) {
        $strMensagem = "
        <p>Prezado(a),<br/>
        Informamos que foi criada uma Restrição para seu Município, abaixo seguem demais informações:</p>
        <ul style=\"list-style: none\">
            <li><strong>Tipo de restrição:</strong> {$arrTipoRestricaoFnde['tprdescricao']}</li>
            <li><strong>Descrição:</strong> {$post['resdescricao']}</li>
            <li><strong>Providências:</strong> {$post['resprovidencia']}</li>";
        if ($post['restemporestricao'] == '1') {
            $strMensagem .="
            <li><strong>Prazo da Providência:</strong> Sim</li>
            <li><strong>Data Inicial:</strong> {$post['resdatainicio']}</li>
            <li><strong>Data Final:</strong> {$post['resdatafim']}</li>";
        } else {
            $strMensagem .="
            <li><strong>Prazo da Providência:</strong> Não</li>";
        }

        $strMensagem .="
        </ul>

        <br>
        Atenciosamente,
        <br>
        Equipe PAR "
        ;
        $strAssunto = "[SIMEC-PAR] Restrições FNDE";
        $remetente = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
        $destinatario = array("usuemail" => $email['usuemail'], "usunome" => $email['usunome']);
        $mensagem = html_entity_decode($strMensagem);
        enviar_email($remetente, $destinatario, $strAssunto, $mensagem);
    }
}

function anexarArquivo() {
    $campos = array("resid" => $_REQUEST['resid']);
    $file = new FilesSimec("anexorestricao", $campos, 'par');

    if ($_FILES["arquivo"]) {
        $arquivoSalvo = $file->setUpload($_FILES["arquivo"]['name']);
        if ($arquivoSalvo) {
            echo "
                <script>
                    alert('Arquivo Anexado com sucesso!');
                    window.location.href = window.location.href;
                </script>";
        }
    }
}

function excluirArquivo() {
    global $db;

    $sql = "
            UPDATE 
                par.anexorestricao 
            SET
                areststatus = 'I'
            WHERE
                arqid = '" . $_REQUEST['arqid'] . "'
        ";

    $db->executar($sql);
    $db->commit();

    echo "
        <script type=\"text/javascript\">
            alert('Arquivo Excluído.');
            document.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&resid=" . $_REQUEST['resid'] . "';
        </script>";
}

function baixarArquivo() {

    $file = new FilesSimec(null, null, 'par');
    return $file->getDownloadArquivo($_REQUEST['arqid']);

    echo '
            <script type="text/javascript">
                document.location.href = document.location.href;
            </script>';
}

if (!empty($resid)) {
    $sql = "
            SELECT
                res.resid,
                CASE WHEN re.inuid IS NULL THEN
                    'T'
                WHEN i.estuf IS NOT NULL THEN
                    'E'
                ELSE
                    'M'
                END AS esfera,
                res.resdescricao,
                res.resdatainicio,
                res.resdatafim,
                res.restemporestricao,
                res.usucpf,
                res.resstatus,
                res.tprid,
                res.resinformar,
                res.resprovidencia,
                res.ressuperado,
                res.resfluxo,
                res.docid
            FROM
                par.restricaofnde res
                LEFT JOIN par.restricaoentidade re ON(res.resid = re.resid)
                LEFT JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
            WHERE
                res.resid = {$resid} ";

    $restricao = $db->pegaLinha($sql);
    # Termo
    $listaFiltro = array('resid' => $resid, 'checked' => TRUE );
    $sqlTermos = listarTermo($listaFiltro, 'SQL');
    $restricao['listaTermo'] = $db->carregar($sqlTermos);

    $sqlEstados = "
            SELECT DISTINCT
            	e.estuf,
                e.estdescricao
            FROM
                par.restricaofnde res
                JOIN par.restricaoentidade re ON(res.resid = re.resid)
                JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
                JOIN territorios.estado e ON(i.estuf = e.estuf)
            WHERE
                res.resid = {$resid}";
    $restricao['listaEstado'] = $db->carregar($sqlEstados);

    # Municiío
    $sqlMunicipios = "
            SELECT DISTINCT
            	m.muncod,
                m.mundescricao || ' - ' || m.estuf AS descricao
            FROM
                par.restricaofnde res
                JOIN par.restricaoentidade re ON(res.resid = re.resid)
                JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
                JOIN territorios.municipio m ON(i.muncod = m.muncod)
            WHERE
                res.resid = {$resid}";
    $restricao['listaMunicipio'] = $db->carregar($sqlMunicipios);
}

monta_titulo('Cadastro/Edição de Restrição FNDE', '');
?>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value="salvar" />
    <input type="hidden" id="resid" name="resid" value="<?php echo $restricao['resid']; ?>" />
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <?php
        if($resid):
        ?>
            <tr>
                <td class="SubTituloDireita" valign="top"><b>Código:</b></td>
                <td>
                    <?php echo $resid ?>
                </td>
            </tr>
        <?php
        endif;
        ?>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Restrição para</b></td>
            <td>
                <input type="radio" id="chkEsferaTermo" value='T' name="esfera" <?= $restricao['esfera'] == 'T' ? 'checked="checked"' : NULL; ?> class="link"  onmouseover="return escape('Restrição para');" >
                <label for="chkEsferaTermo" class="link">Termo</label>

                <input type="radio" id="chkEsferaMunicipal" value='E' name="esfera" <?= $restricao['esfera'] == 'E' ? 'checked="checked"' : NULL; ?> class="link" onmouseover="return escape('Restrição para');" >
                <label for="chkEsferaMunicipal" class="link">Estado</label>

                <input type="radio" id="chkEsferaEstado" value='M' name="esfera" <?= $restricao['esfera'] == 'M' ? 'checked="checked"' : NULL; ?> class="link" onmouseover="return escape('Restrição para');" >
                <label for="chkEsferaEstado" class="link">Município</label>
                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr class="trRestricaoTermo" style="display: none;">
            <td class="SubTituloDireita" valign="top"><b>Lista de Termos</b></td>
            <td>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom: none;">
                    <thead>
                        <tr>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Ação
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Termo
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Tipo do Termo
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Vigência
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Banco
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Agência
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Conta Corrente
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Valor Pago(R$)
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Processo
                                </strong>
                            </th>
                            <th valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong>
                                    Tipo do Processo
                                </strong>
                            </th>
                        </tr>
                    </thead>
                    <tbody id="tbodyTermo">
                        <?php
                        if (!empty($restricao['listaTermo'])):
                            foreach ($restricao['listaTermo'] as $termo):
                                ?>
                                <tr id="tr_termo_<?php echo $termo['dopid']; ?>">
                                    <td><?php echo $termo['acao']; ?></td>
                                    <td align="right" style="color:#0066cc;"><?php echo $termo['numero_do_termo']; ?></td>
                                    <td><?php echo $termo['tipo_termo']; ?></td>
                                    <td><?php echo $termo['vigencia']; ?></td>
                                    <td align="right" style="color:#0066cc;"><?php echo $termo['banco']; ?></td>
                                    <td align="right" style="color:#0066cc;"><?php echo $termo['agencia']; ?></td>
                                    <td align="right" style="color:#0066cc;"><?php echo $termo['conta_corrente']; ?></td>
                                    <td><?php echo $termo['valor_pago']; ?></td>
                                    <td align="right" style="color:#0066cc;"><?php echo $termo['processo']; ?></td>
                                    <td><?php echo $termo['tipo_do_processo']; ?></td>
                                </tr>
                                <?php
                            endforeach;
                        endif;
                        ?>
                    </tbody>
                </table>
            </td>
        </tr>
        <tr class="trRestricaoEstado" style="display: none;">
            <td class="SubTituloDireita" valign="top"><b>UF</b></td>
            <td>
                <?php
                $sqlBuscaEstados = "
                    SELECT 
                        estuf AS codigo,
                        estdescricao AS descricao 
                    FROM 
                        territorios.estado
                    ORDER BY 
                        estuf;
                ";

                $arrEstados = array();
                if (is_array($restricao['listaEstado'])) {
                    foreach ($restricao['listaEstado'] as $key => $estado) {
                        $arrEstados[$key]['codigo'] = $estado['estuf'];
                        $arrEstados[$key]['descricao'] = $estado['estdescricao'];
                    }
                }

                combo_popup("estuf", $sqlBuscaEstados, "Lista de Estados", "400x400", 0, array(), "", "S", false, false, 5, 400, null, null, false, null, $arrEstados);
                ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr class="trRestricaoMunicipio" style="display: none;">
            <td class="SubTituloDireita" valign="top"><b>Municípios</b></td>
            <td>
                <div onmouseover="return escape('Municípios');">
                    <select
                        multiple="multiple"
                        size="5"
                        name="listaMunicipio[]"
                        id="listaMunicipio"
                        ondblclick="abrirPopupListaMunicipio();"
                        class="CampoEstilo link"
                        onkeydown="javascript:combo_popup_remove_selecionados(event, 'listaMunicipio');"
                        style="width:400px;" >
                            <?php if (!empty($restricao['listaMunicipio'])): ?>
                                <?php foreach ($restricao['listaMunicipio'] as $municipio): ?>
                                <option value="<?php echo $municipio['muncod']; ?>" selected="selected"><?php echo $municipio['descricao']; ?></option>
                            <?php endforeach; ?>
                        <?php else: ?>
                            <option value="">Duplo clique para selecionar da lista</option>
                        <?php endif; ?>
                    </select>
                </div>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Tipo da restrição FNDE</b></td>
            <td>
                <?php
                $sql = "
                        SELECT DISTINCT
                            tpr.tprid AS codigo,
                            tpr.tprdescricao AS descricao
                        FROM 
                            par.tiporestricaofnde tpr
                        WHERE
                            tpr.tprstatus = 'A'
                        ORDER BY
                            tpr.tprdescricao ASC
                    ";
                $db->monta_combo("tprid", $sql, 'S', 'Selecione...', '', '', 'Tipo da restrição FNDE', '', '', 'tprid', '', $restricao['tprid'], '', '', ' link ');
                ?>
                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
            <td>
                <?php echo campo_textarea('resdescricao', 'S', 'S', 'Descrição', 100, 5, '', '', 'left', 'Descrição', 0, '', $restricao['resdescricao']); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Informar Município/Estado:</b></td>
            <td>
                <?php
                if ($restricao['resinformar'] == 'S')
                    $resinformar_sim = ' checked="checked" ';
                if ($restricao['resinformar'] == 'N')
                    $resinformar_nao = ' checked="checked" ';
                ?>
                <input type="radio" id="resinformar_sim" value='S' <?php echo $resinformar_sim; ?> name="resinformar" class="link" onmouseover="return escape('Informa se o município ou estado terá um prazo para informar a providência.');" >
                <label for="resinformar_sim" class="link">Sim</label>

                <input type="radio" id="resinformar_nao" value='N' <?php echo $resinformar_nao; ?> name="resinformar" class="link" onmouseover="return escape('Informa se o município ou estado terá um prazo para informar a providência.');" >
                <label for="resinformar_nao" class="link">Não</label>
                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Providências:</b></td>
            <td>
                <?php echo campo_textarea('resprovidencia', 'S', 'S', 'Providências', 100, 5, '', '', 'left', 'Providências', 0, '', $restricao['resprovidencia']); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Prazo da providência:</b></td>
            <td>
                <?php
                if ($restricao['restemporestricao'] == 't')
                    $restemporestricao_sim = ' checked="checked" ';
                if ($restricao['restemporestricao'] == 'f')
                    $restemporestricao_nao = ' checked="checked" ';
                ?>
                <input type="radio" id="restemporestricao_sim" value='1' name="restemporestricao" <?php echo $restemporestricao_sim; ?> class="link" onmouseover="return escape('Informa se o município ou estado terá um prazo para informar a providência.');" >
                <label for="restemporestricao_sim" class="link">Sim</label>

                <input type="radio" id="restemporestricao_nao" value='0' name="restemporestricao" <?php echo $restemporestricao_nao; ?> class="link" onmouseover="return escape('Informa se o município ou estado terá um prazo para informar a providência.');" >
                <label for="restemporestricao_nao" class="link">Não</label>
                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr style="display: none;" class="trDataProvidencia">
            <td class="SubTituloDireita" valign="top"><b>Data de início</b></td>
            <td>
                <?= campo_data('resdatainicio', 'N', 'S', '', 'S', 'Data de inicio do prazo para o município ou estado informar uma providência.', '', $restricao['resdatainicio']); ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr style="display: none;" class="trDataProvidencia">
            <td class="SubTituloDireita" valign="top"><b>Data de fim</b></td>
            <td>
                <?= campo_data('resdatafim', 'N', 'S', '', 'S', 'Data final do prazo para o município ou estado informar uma providência.', '', $restricao['resdatafim']); ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Alerta de E-mail:</b></td>
            <td>
                <?php
                if ($restricao['ressuperado'] == 'S')
                    $ressuperado_sim = ' checked="checked" ';
                if ($restricao['ressuperado'] == 'N')
                    $ressuperado_nao = ' checked="checked" ';
                ?>
                <input type="radio" id="ressuperado_sim" value='S' <?php echo $ressuperado_sim; ?> name="ressuperado" class="link" onmouseover="return escape('Informa se ao término do prazo de providência o técnico e coordenador receberão um e-mail de aviso.');" >
                <label for="ressuperado_sim" class="link">Sim</label>

                <input type="radio" id="ressuperado_nao" value='N' <?php echo $ressuperado_nao; ?> name="ressuperado" class="link" onmouseover="return escape('Informa se ao término do prazo de providência o técnico e coordenador receberão um e-mail de aviso.');" >
                <label for="ressuperado_nao" class="link">Não</label>
                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Arquivos:</b></td>
            <td id="tdAnexo">
                <input type="file" name="arquivo" class="inputArquivo link" />
                <?php if (!empty($restricao['resid'])): ?>
                    <input id="btnAnexar" type="button" value="Anexar" />
                <?php endif; ?>
                <br />
                <br />
                <?php
                $acoes = '\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="excluirArquivo">
                        <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
                        </a>&nbsp;\'||
                        \'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
                        \' || arq.arqdescricao || \'
                        </a>\'
                    ';

                if (!empty($restricao['resid'])) {
                    $cabecalho = array('Arquivo');

                    $sql = "
                            SELECT
                                $acoes as acoes
                            FROM
                                par.anexorestricao ares
                                JOIN public.arquivo arq ON arq.arqid = ares.arqid
                                JOIN par.restricaofnde res ON ares.resid = res.resid
                            WHERE
                                ares.areststatus = 'A'
                                AND res.resid = {$restricao['resid']}
                        ";

                    $arrArquivos = $db->carregar($sql);
                    $arrArquivos = $arrArquivos ? $arrArquivos : array();

                    if (is_array($arrArquivos)) {
                        $db->monta_lista($arrArquivos, $cabecalho, 50, 10, '', 'center', 'N', '', '', '', '', '');
                    }
                }
                ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="submit" id="btnSalvar" value="Salvar" />
                &nbsp;
                <input type="button" id="btnCancelar" value="Limpar" />
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
    $(document).ready(function() {
        // Selecionar Estados
        $( "#estuf" ).bind( "change blur focusout mouseover focusin", function() {
            $('#estuf option').attr('selected', 'selected');
        }); 
    
        $('#btnCancelar').click(function() {
            window.location.reload();
        });

        $('input[name=esfera]').click(function() {
            ocultarElementosEsfera();
            exibirElementosEsfera();
            if ($('input[NAME=esfera]:checked').val() == 'T') {
                abrirPopupSelecionarTermo();
            }
        });

        $('input[name=restemporestricao]').click(function() {
            ocultarElementosProvidencia();
            exibirElementosProvidencia();
        });

        $('input.listaTermo').live('change', function() {
            if ($(this).attr('checked') != true) {
                $(this).parent().parent().remove();
            }
        });

        $('#btnAnexar').click(function() {
            var resid = $('#resid').val();
            if (resid != '') {
                $('#requisicao').val('anexarArquivo');
                $('#formulario').submit();
            }
        });

        // Baixar Arquivos
        $('.downloadArquivo').click(function() {
            window.open('par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&requisicao=baixarArquivo&arqid=' + $(this).attr('id'), 'Arquivo', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        });

        // Excluir Arquivos
        $('.excluirArquivo').click(function() {
            if (confirm('Deseja excluir este arquivo?')) {
                window.location.href = window.location.href + '&requisicao=excluirArquivo&arqid=' + $(this).attr('id') + '&resid=' + $('#resid').val();
            }
        });

        // Validação do formulário
        $('#formulario').submit(function()
        {
            var esfera = $('input[name="esfera"]:checked').val();

            if (esfera == undefined || esfera == null || esfera == '') {
                alert('Selecione a esfera para restrição.');
                return false;
            } else {
                if (esfera == 'M') {
                    var municipios = $('#listaMunicipio').val();
                    if (municipios == undefined || municipios == null || municipios == '') {
                        alert('Escolha um municipio.');
                        return false;
                    }
                } else if (esfera == 'E') {
                    $('#estuf option').attr('selected', 'selected');
                    var estados = $('#estuf option:selected').val();

                    if (estados == undefined || estados == null || estados == '') {
                        alert('Escolha um estado.');
                        return false;
                    }
                } else {
                    var termos = $('[name^="listaTermo"]').val();
                    if (termos == undefined || termos == null || termos == '') {
                        alert('Escolha um termo.');
                        return false;
                    }
                }
            }

            var tprestricao = $('#tprid').val();

            if (tprestricao == undefined || tprestricao == null || tprestricao == '') {
                alert('Selecione um tipo de restrição.');
                return false;
            }

            var descricao = $('#resdescricao').val();

            if (descricao == undefined || descricao == null || descricao == '') {
                alert('Informe uma descrição.');
                return false;
            }

            var informar = $('input[name="resinformar"]:checked').val();

            if (informar == undefined || informar == null || informar == '') {
                alert('Informe se município/estado deve informado sobre restrição.');
                return false;
            }

            var providencias = $('#resprovidencia').val();

            if (providencias == undefined || providencias == null || providencias == '') {
                alert('Informe uma providência.');
                return false;
            }

            var prazoProvidencia = $('input[name="restemporestricao"]:checked').val();

            if (prazoProvidencia == undefined || prazoProvidencia == null || prazoProvidencia == '') {
                alert('Informe um prazo da providência.');
                return false;
            } else {
                if (prazoProvidencia == '1' || prazoProvidencia == 1) {
                    var resdatainicio = $('#resdatainicio').val();
                    var resdatafim = $('#resdatafim').val();

                    if (resdatainicio == undefined || resdatainicio == null || resdatainicio == '') {
                        alert('Informe uma data de inicio para o prazo da providência');
                        return false;
                    }

                    if (resdatafim == undefined || resdatafim == null || resdatafim == '') {
                        alert('Informe uma data de fim para o prazo da providência');
                        return false;
                    }
                }
            }

            var superado = $('input[name="ressuperado"]:checked').val();

            if (superado == undefined || superado == null || superado == '') {
                alert('Informe superado.');
                return false;
            }

            return true;
        });

        ocultarElementosEsfera();
        exibirElementosEsfera();

        ocultarElementosProvidencia();
        exibirElementosProvidencia();

    });

    /**
     * Oculta os elementos do formulario oriundos do tipo de restrição
     * 
     * @returns VOID
     */
    function ocultarElementosEsfera() {
        $('.trRestricaoTermo').hide();
        $('.trRestricaoEstado').hide();
        $('.trRestricaoMunicipio').hide();
    }

    /**
     * Oculta os elementos do formulario oriundos da opção de providencia
     * 
     * @returns VOID
     */
    function ocultarElementosProvidencia() {
        $('.trDataProvidencia').hide();
    }

    /**
     * Controla exibição de elementos do formulario de acordo com o tipo de restrição
     * selecionada
     * 
     * @returns VOID
     */
    function exibirElementosEsfera() {
        var tpEsfera = $('input[NAME=esfera]:checked').val();
        switch (tpEsfera) {
            case 'T':
                $('.trRestricaoTermo').show();
                break;
            case 'E':
                $('.trRestricaoEstado').show();
                break;
            case 'M':
                $('.trRestricaoMunicipio').show();
                break;
        }
    }

    /**
     * Exibe a tela modal para selecionar o termo
     * 
     * @returns VOID
     */
    function abrirPopupSelecionarTermo() {
        var resid = $('#resid').attr('resid');
        window.open('par.php?modulo=sistema/tabelasapoio/restricoes_fnde/selecionarTermo&acao=A&resid=' + resid, 'selecionarTermo', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes,width=900,height=600');
    }

    /**
     * Controla exibição de elementos do formulario de acordo com o tipo de restrição
     * selecionada
     * 
     * @returns VOID
     */
    function exibirElementosProvidencia() {
        var tpProvidencia = $('input[NAME=restemporestricao]:checked').val();
        if (tpProvidencia == '1') {
            $('.trDataProvidencia').show();
        } else {
            ocultarElementosProvidencia();
        }
    }

    /**
     * Retorna pra pagina de lista
     * 
     * @returns VOID
     */
    function cancelar() {
        window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/restricaoFNDE&acao=A';
    }

    /**
     * Abre janela popup para selecionar municipios
     * 
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }

    /**
     * Abre janela popup para selecionar municipios
     * 
     * @returns VOID
     */
    function abrirPopupListaEstado() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }

    function consultarTermo(terid) {
        window.open('par.php?modulo=principal/gerarTermoObra&acao=A&requisicao=download&terid=' + terid,
                'modelo',
                "height=600,width=400,scrollbars=yes,top=0,left=0");
    }
    
    function consultarTermoPorDopid(dopid) {
        window.open('par.php?modulo=principal/documentoPar&acao=A&req=formVizualizaDocumento&dopid='+dopid, 
                    'modelo', 
                    "height=600,width=400,scrollbars=yes,top=0,left=0" );
    }
</script>