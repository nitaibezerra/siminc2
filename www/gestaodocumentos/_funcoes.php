<?PHP

include_once "config.inc";
include_once "_constantes.php";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if($_REQUEST['funcao'] != '') {
    $_REQUEST['funcao']($_REQUEST);
    die();
}

require_once APPRAIZ . "includes/classes/dateTime.inc";

function mensagemAcossiacao(){
    ?>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
    align="center">
    <tr>
      <td class="SubTituloCentro" align="center"><font color="red"><?PHP echo 'É necessário Associar a uma Unidade' ?></font></td>
  </tr>
</table>

<?PHP
die;
}

#Montar árvore
function montarArvore($_tartarefa = null, $boCarregaLinkAjax = false, $boSomenteTabela = false, $acomp = 'N' ){
	global $db;

	echo "<div id=\"lista\">";
	echo "<table id=\"tabela_tarefa\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" align=\"center\">";
	echo "<tr style=\"background-color: #e0e0e0\">
    <td style=\"font-weight:bold; text-align:center; width:5%;\">Ação</td>
    <td style=\"font-weight:bold; text-align:center; width:4%;\">Prioridade</td>
    <td style=\"font-weight:bold; text-align:center; width:20%;\">Identificação da Demanda</td>
    <td style=\"font-weight:bold; text-align:center; width:18%;\">Solicitante</td>
    <td style=\"font-weight:bold; text-align:center; width:3%;\">Nível</td>
    <td style=\"font-weight:bold; text-align:center; width:5%;\">Dias Decorridos</td>
    <td style=\"font-weight:bold; text-align:center; width:20%;\">Responsável</td>
    <td style=\"font-weight:bold; text-align:center; width:7%;\">Situação</td>
    <td style=\"font-weight:bold; text-align:center; width:7%;\">Prazo de Atendimento</td>
    ";
    if($acomp == 'N'){
        echo "<td style=\"font-weight:bold; text-align:center; width:12%;\">Status</td>";
    }
    echo "</tr>";
    echo "</table>
    </div>";
    if(!$boSomenteTabela){
      echo "<script type=\"text/javascript\">
      montaPai('$_tartarefa', '', '$boCarregaLinkAjax');
      </script>";
  }
}

#É CHAMADO NA TELA CADASTRO DE DEMANDAS - DELETA OS OBJETOS DA DEMANDA.
function deletaObjDemanda($dados){
    global $db;

    $ojdid  = trim($dados['ojdid']);

    $sql = "
    DELETE FROM gestaodocumentos.objetodemanda WHERE ojdid = {$ojdid};
    ";
    $dados = $db->pegaUm($sql);

    if ( $db->executar($sql) ) {
        $db->commit();
        die("<resp>OK</resp>");
    } else {
        die("<resp>ERRO</resp>");
    }
}

#DELETAR REGISTRO DE REITERAÇÃO - ESPEFICICADO PELO ID DA TABELA.
function deletaReiteracao($dados){
    global $db;

    require_once APPRAIZ . "gestaodocumentos/classes/Reiteracao.class.inc";

    $rtrid = $_REQUEST['rtrid'];

    $objReiteracaoes = new Reiteracao();

    if( $rtrid != '' ){
        $objReiteracaoes->deletarReiteracaoPorRtrid( $rtrid );
        $db->commit();
        die("<resp>OK</resp>");
    }
}

#É CHAMADO NA TELA CADASTRO DE DEMANDAS - DELETA OS SOLICITANTES.
function deletaSolicitante($dados){
    global $db;

    $issid  = trim($dados['issid']);

    $sql = "
    DELETE FROM gestaodocumentos.instituicaosolicitante WHERE issid = {$issid};
    ";
    $dados = $db->pegaUm($sql);

    if ( $db->executar($sql) ) {
        $db->commit();
        die("<resp>OK</resp>");
    } else {
        die("<resp>ERRO</resp>");
    }
}

function buscaDadosReiteracao($tarid){
    global $db;

    $sql = "
        SELECT  r.rtrid,
                r.taridprincipal,
                r.taridsecundario,
                t.tartiponumsidoc,
                CASE
                    WHEN char_length(trim(tarnumsidoc)) = 12 THEN  substr(tarnumsidoc,0,7) || '.' || substr(tarnumsidoc,7,4) || '-' || substr(tarnumsidoc,11,2)
                    WHEN char_length(trim(tarnumsidoc)) = 17 THEN substring(tarnumsidoc,0,6) || '.' || substr(tarnumsidoc,6,6) || '/' || substr(tarnumsidoc,12,4) || '-' || substr(tarnumsidoc,16,2)
                    ELSE ''
                END AS tarnumsidoc
        FROM gestaodocumentos.reiteracoes AS r

        JOIN gestaodocumentos.tarefa AS t ON t.tarid = r.taridprincipal

        WHERE r.taridsecundario =  {$tarid}
    ";
    $dados = $db->carregar($sql);

    return $dados;
}


function blocoDadosTarefa(&$obTarefa, &$arCadTarefa = null, &$instituicoesSelecionadas = null) {
    global $db;

    $permissao = buscaPermissaoPerfilSalvar( $obTarefa->tarid );
    if( $permissao == 'S' ){
        $habilitado = 'S';
        $disabled = '';
    }else{
        $habilitado = 'N';
        $disabled = 'disabled="disabled"';
    }

    $externo = $obTarefa->tarsitprocexterno;
    if( $externo == 't' ){
        $habilita_Externa= 'S';
    }elseif( $externo == 'f' ){
        $habilita_Externa= 'N';
    }else{
        $habilita_Externa= 'N';
    }

    function formatarSidocProcesso($sidoc) {
        if(empty($sidoc)){
            return ;
        } else {
            return substr($sidoc,0,5).".".substr($sidoc,5,6)."/".substr($sidoc,11,4)."-".substr($sidoc,15,2);
        }
    }

    function formatarSidocDocumento($sidoc) {
        if(empty($sidoc)){
            return ;
        } else {
            return substr($sidoc,0,6).".".substr($sidoc,6,4)."-".substr($sidoc,10,2);
        }
    }

    ?>
    <tr>
        <td align="left" colspan="4"><b>DADOS DA DEMANDA</b></td>

        <td rowspan="8" valign="top" align="left" width="45%">
            <?PHP
            if( $obTarefa->tarid != ''){
                $reiteracao = buscaDadosReiteracao( $obTarefa->tarid );
                if($reiteracao[0]['taridprincipal'] != ''){
                    ?>
                    <fieldset style="height: 180px;">
                        <legend>Reiteração</legend>
                        <br>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                            <tr>
                                <td class="subTituloCentro">Origem</td>
                                <td class="subTituloCentro">SIDOC</td>
                            </tr>
                            <? foreach ($reiteracao as $dados){ ?>
                            <tr>
                                <td class="subTitulo" style="text-align: center;"><?=$dados['taridprincipal'];?></td>
                                <td class="subTitulo" style="text-align: center;"><a style="text-decoration:underline;" href="gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid=<?=$dados['taridprincipal']?>"><?=$dados['tarnumsidoc'];?></a></td>
                            </tr>
                            <? } ?>
                        </table>
                    </fieldset>
                    <?PHP
                }
            }
            ?>
        </td>

        <td align="center" style="background-color: #f0f0f0; vertical-align: 0px;" rowspan="30">
            <?PHP
            require_once APPRAIZ . "includes/workflow.php";
            $tarid = $obTarefa->tarid;

            if($tarid != ''){
                $docid = buscarDocidGestaoDocumentos( $tarid );

                if($docid != ''){
                    $dados_wf = array("tarid" => $tarid,"docid" => $docid);
                    wf_desenhaBarraNavegacao($docid, $dados_wf);
                }
            }
            ?>
        </td>
    </tr>

    <tr>
    	<td width="20%" class="SubTituloDireita">Identificação da Demanda:</td>
    	<td colspan="3">
            <?PHP
            $tartitulo = $obTarefa->tartitulo;
            echo campo_texto('tartitulo', 'S', $habilitado, 'Identificação da Demanda', 71, 255, '', '', '', '', '', 'id="tartitulo"', '', $tartitulo);
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" align="right">Expediente:</td>
        <td colspan="3">
            <?PHP
            $sql = "
            SELECT  tpeid AS codigo,
            tpedsc AS descricao
            FROM gestaodocumentos.tipoexpediente
            ORDER BY descricao
            ";
            $tpeid = $obTarefa->tpeid;
            $db->monta_combo('tpeid', $sql, $habilitado, "Selecione...", '', '', '', '520', 'S', 'tpeid', false, $tpeid, 'Expediente' );
            ?>
        </td>
    </tr>
    <tr>
    	<td class="SubTituloDireita" align="right">Data do Recebimento:</td>
    	<td colspan="3">
            <?PHP
            $tardatarecebimento = ($obTarefa->tardatarecebimento) ? $obTarefa->tardatarecebimento : date("Y/m/d");
            if ($arCadTarefa['tardatarecebimento']) {
                $obData = new Data();
                $tardatarecebimento = $obData->formataData($tardatarecebimento, "YYYY-mm-dd");
            }
            echo campo_data2('tardatarecebimento', 'S', $habilitado, 'Data do Recebimento', 'S', '', 'validaDataRecebimento(this)', $tardatarecebimento);
            ?>
        </td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Número SIDOC:</td>
        <?PHP
        $tarnumsidoc = $obTarefa->tarnumsidoc;
        $tartiponumsidoc = $obTarefa->tartiponumsidoc;
        ?>

        <input type="hidden" name="hid_tarnumsidoc" value="<?PHP echo $tarnumsidoc; ?>" id="hid_tarnumsidoc">
        <input type="hidden" name="hid_tartiponumsidoc" value="<?PHP echo $tartiponumsidoc; ?>" id="hid_tartiponumsidoc">

        <td colspan="3">
            <input type="radio" name="tartiponumsidoc" id="tartiponumsidoc" value="D" <?PHP if($tartiponumsidoc == 'D') echo 'checked=checked'; ?> onclick="mudarTipoSIDOC('D');"> &nbsp;Documento
            &nbsp;&nbsp;&nbsp;&nbsp;
            <input type="radio" name="tartiponumsidoc" id="tartiponumsidoc" value="P" <?PHP if($tartiponumsidoc == 'P') echo 'checked=checked'; ?> onclick="mudarTipoSIDOC('P');"> &nbsp;Processo<br>
            <div id="div_numerosidoc_documento" style="display:none;"><input type="text" size="25" name="tarnumsidoc" id="tarnumsidoc_doc" maxlength="14" onkeyup="this.value=mascaraglobal('######.####-##',this.value);" value="<?PHP echo formatarSidocDocumento($tarnumsidoc); ?>"></div>
            <div id="div_numerosidoc_processo" style="display:none;"><input type="text" size="25" name="tarnumsidoc" id="tarnumsidoc_proc" maxlength="20" onkeyup="this.value=mascaraglobal('#####.######/####-##',this.value);" value="<?PHP echo formatarSidocProcesso($tarnumsidoc);?>"></div>
        </td>
    </tr>
    <tr>
    	<td class="SubTituloDireita" valign="top">Ano Base:</td>
        <td colspan="3">
            <?PHP
            $taranobase = $obTarefa->taranobase;
            if(empty($taranobase)){
               $taranobase = date('Y');
           }

           echo campo_texto('taranobase', 'S', $habilitado, 'Ano Base', 11, 4, '####', '', '', '', '', 'id="taranobase"', '', $taranobase); ?>
       </td>
   </tr>
   <tr>
       <td class="SubTituloDireita">Identificação Externa:</td>
       <td width="10%">
        <?PHP $tarsitprocexterno = $obTarefa->tarsitprocexterno; ?>
        <input type="radio" name="tarsitprocexterno" id="tarsitprocexterno" value="1" title="Identificação Externa" onclick="habilitaCamposExterna('S');" <? if ($tarsitprocexterno == 't') echo 'checked=checked'; ?> > Sim
        <input type="radio" name="tarsitprocexterno" id="tarsitprocexterno" value="0" title="Identificação Externa" onclick="habilitaCamposExterna('N');" <? if ($tarsitprocexterno == 'f') echo 'checked=checked'; ?> > Não
    </td>
    <td width="35%">
        Nº do Documento:<br>
        <?PHP
        $tarnumidentexterno = trim($obTarefa->tarnumidentexterno);
        echo campo_texto('tarnumidentexterno', 'N', $habilita_Externa, 'Nº da Identificação', 42, 50, '', '', '', '', '', 'id="tarnumidentexterno"', '', $tarnumidentexterno);
        ?>
        <br><br>
        Nº do Processo:<br>
        <?PHP
        $tarnumprocexterno = trim($obTarefa->tarnumprocexterno);
        echo campo_texto('tarnumprocexterno', 'N', $habilita_Externa, 'Nº do Processo', 42, 50, '', '', '', '', '', 'id="tarnumprocexterno"', '', $tarnumprocexterno);
        ?>
    </td>
</tr>

<?PHP
    echo blocoDadosSolicitante($obTarefa, $arCadTarefa);

    if(!$boCadAcompanhamento){
        if($obTarefa->tarid){
            $taridprincipal = $obTarefa->tarid;
        } else {
            $taridprincipal = 0;
        }
    }
?>
    <tr>
        <td rowspan="2"class="SubTituloDireita">Reiteração:</td>
        <td colspan="4">
<?PHP
            if($obTarefa->tarid){
                $taridr = $obTarefa->tarid;
            } else {
                $taridr = 0;
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaReiteracao('||r.rtrid||')\" title=\"Deletar Reiteração\" >
                </center>
            ";

            $sql = "
                SELECT  '{$acao}' as acao,
                        '<a style=\"text-decoration:underline;\" href=\"gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid=' || r.taridsecundario || '\">' || tarnumsidoc || '</a>' as tarnumsidoc,
                        tartitulo
                FROM gestaodocumentos.reiteracoes r
                LEFT JOIN gestaodocumentos.tarefa t ON t.tarid = r.taridsecundario

                WHERE r.taridprincipal = {$taridr}

                ORDER BY tartitulo
            ";
            $cabecalho = array( "Ação", "Nº SIDOC/EMEC", "Identificação" );
            $alinhamento = Array('center', 'left', 'left', 'center');
            $param['totalLinhas'] = false;
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento, null, $param); ?>
        </td>
    </tr>

<?PHP if($obTarefa->tarid){ ?>
<tr>
    <td colspan="4" class="subTituloCentro">
        <input type="button" value="Anexar processo" onclick="abreReiteracaoProcesso(<?PHP echo $obTarefa->tarid; ?>);">
    </td>
</tr>
<?PHP } else { ?>
<tr>
   <td class="subTituloCentro">&nbsp;</td>
   <td colspan="3" class="subTituloCentro" align="center">SALVE A DEMANDA PARA ADICIONAR A REITERAÇÃO.</td>
</tr>
<?PHP } ?>
<tr>
    <td class="SubTituloDireita">Setor de Origem:</td>
    <td colspan="4">
     <?PHP
     $unaidsetororigem = $obTarefa->unaidsetororigem;
     $sql = "
     SELECT  unaid as codigo,
     unadescricao as descricao
     FROM gestaodocumentos.unidade
     ORDER BY unadescricao
     ";
     $db->monta_combo("unaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', '520', 'S', 'unaidsetororigem', false, $unaidsetororigem, 'Setor de Origem');
     ?>
 </td>
</tr>
<tr>
    <td class="SubTituloDireita">Setor Responsável:</td>
    <td colspan="4">
        <b>Coordenação-Geral de Legislação e Normas de Regulação e Supervisão da Educação Superior - CGLNRS/SERES</b>
    </td>
</tr>
<?PHP if($obTarefa->tarid && !$boCadAcompanhamento){ ?>
<tr>
    <td class="SubTituloDireita">Expressão Chave:</td>
    <td colspan="4">
        <table id="tabela_expressao" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
            <tr>
                <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Ação</strong>
                </td>
                <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Expressão Chave</strong>
                </td>
            </tr>
            <tr>
                <!-- LISTAGEM DA PALAVRA CHAVE -->
                <?PHP
                if($obTarefa->tarid){
                    $sql = "
                    SELECT *
                    FROM gestaodocumentos.palavrachave
                    WHERE tarid  = ".$obTarefa->tarid."
                    ORDER BY plcdsc;
                    ";
                    $pchave = $db->carregar($sql);
                    if( $pchave[0] ) {
                            //echo "<tr><td bgcolor=\"#ffffff\">Ação</th><th>Expressão Chave</th></tr>";
                        foreach($pchave as $in) {
                            $acoes = "
                            <input type='hidden' name='expressaochave[]' value='".$in['plcdsc']."'>
                            <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick='editarExpressao(this.parentNode.parentNode.parentNode.rowIndex);'/>
                            <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarExpressao(this.parentNode.parentNode.parentNode.rowIndex);\"/>
                            ";
                            echo "<tr>";
                            echo "<td><center>".$acoes."</center></td>";
                            echo "<td>".$in['plcdsc']."</td>";
                        }
                    }
                }
                ?>
                <!-- FIM - LISTAGEM DA PALAVRA CHAVE -->
            </tr>
            <tr>
                <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <img style="cursor:pointer;" src="../imagens/gif_inclui.gif"  onclick="cadastrarExpressao();">
                </td>
                <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <input type="text" class="normal" id="expressao_chave" size="60" maxlength="30" value="<?=$_REQUEST["expressao_chave"]?>">
                </td>
            </tr>
        </table>
    </td>
</tr>
<?PHP } ?>
<tr>
    <td align="left" colspan="5"><b>DADOS DA TAREFA</b></td>
</tr>
<tr>
    <td class="SubTituloDireita">Responsável pela Demanda:</td>
    <td id="td_usucpfresponsavel2" colspan="4">
        <?PHP
        $sql = "
        SELECT  distinct u.usucpf AS codigo,
        CASE WHEN t.qdt_dem IS NULL
        THEN u.usunome ||' - QTD: 0'
        ELSE u.usunome ||' - QTD: '||t.qdt_dem
        END AS descricao
        FROM seguranca.usuario AS u
        JOIN seguranca.perfilusuario AS p ON p.usucpf = u.usucpf AND p.pflcod IN (".PERFIL_GDOCUMENTO_APOIO .",". PERFIL_GDOCUMENTO_COORDENACAO_GERAL .",". PERFIL_GDOCUMENTO_DIRETORIA .",". PERFIL_GDOCUMENTO_ESQUIPE_TEC .",". PERFIL_GDOCUMENTO_ADMINISTRADOR .")

        LEFT JOIN (
            SELECT 	COUNT(tarid) AS qdt_dem,
            usucpfresponsavel
            FROM gestaodocumentos.tarefa ta
            LEFT JOIN workflow.documento d ON d.docid = ta.docid AND d.esdid not in (" . WF_PROCESSO_FINALIZADO. "," . WF_PROCESSO_ARQUIVADO . ")
            GROUP BY usucpfresponsavel
            ) AS t ON t.usucpfresponsavel = u.usucpf

ORDER BY 2
";
$usucpfresponsavel = $obTarefa->usucpfresponsavel;
$db->monta_combo('usucpfresponsavel', $sql, $habilitado, 'Selecione...', '', '', '', '520', 'N', 'usucpfresponsavel', false, $usucpfresponsavel );
?>
</td>
<input type="hidden" value="<?PHP echo $usucpfresponsavel; ?>" name="usucpfresponsavelAnterior" id="usucpfresponsavelAnterior">
</tr>
<tr>
    <td class="SubTituloDireita">Prioridade:</td>
    <td colspan="4">
        <?PHP
        $arPrioridade = array();
        $arPrioridade['N'] = "Normal";
        $arPrioridade['U'] = "Urgente";
        $arPrioridade['O'] = "Urgente c/ prazo de outras autoridades";
        $arPrioridade['J'] = "Urgente c/ prazo judicial";
        foreach ($arPrioridade as $valor => $prioridade) {
            if ($obTarefa->tarprioridade == $valor) {
                $ckecked = "checked=\"checked\"";
            } else {
                if ($valor == 'N') {
                    $ckecked = "checked=\"checked\"";
                }
            }
            echo "<input type=\"radio\" $disabled $ckecked id=\"{$prioridade}\" name=\"tarprioridade\" title=\"Prioridade\" value=\"{$valor}\" align=\"bottom\"><label for=\"{$prioridade}\">{$prioridade}</label>";
            $ckecked = "";
        }
        ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" align="right">Prazo para Atendimento:</td>
    <td colspan="4">
        <?PHP
        $obData = new Data();
        $tardataprazoatendimento = $obTarefa->tardataprazoatendimento;
        if ($tardataprazoatendimento) {
            $tardataprazoatendimentoAnterior = $obData->formataData($tardataprazoatendimento, "dd/mm/YYYY");
        }
        if ($arCadTarefa['tardataprazoatendimento']) {
            $obData = new Data();
            $tardataprazoatendimento = $obData->formataData($tardataprazoatendimento, "YYYY-mm-dd");
        }
        echo campo_data2('tardataprazoatendimento', 'N', $habilitado, 'Prazo para Atendimento', 'S', '', 'verificaDataPaiEDataFilha(this,'. $taridprincipal.', 1);', $tardataprazoatendimento);
        ?>
        <input type="hidden" value="<?PHP echo $tardataprazoatendimentoAnterior; ?>" name="tardataprazoatendimentoAnterior" id="tardataprazoatendimentoAnterior">
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" align="right">Data do Envio:</td>
    <td colspan="4">
        <?PHP
        $tardatainicio = ($obTarefa->tardatainicio) ? $obTarefa->tardatainicio : date("Y/m/d");
        if ($arCadTarefa['tardatainicio']) {
            $obData = new Data();
            $tardatainicio = $obData->formataData($tardatainicio, "YYYY-mm-dd");
        }
        echo campo_data2('tardatainicio', 'N', 'S', 'Data de Início', 'S', '', '', $tardatainicio);
        ?>
    </td>
</tr>

<script type="text/javascript">
function mudarTipoSIDOC(tipo) {
  if(tipo == 'D'){
     jQuery('#div_numerosidoc_documento').show();
     jQuery('#tarnumsidoc_doc').removeAttr('disabled');
     jQuery('#div_numerosidoc_processo').hide();
     jQuery('#tarnumsidoc_proc').attr('disabled','true');
 } else if(tipo == 'P') {
     jQuery('#div_numerosidoc_processo').show();
     jQuery('#tarnumsidoc_proc').removeAttr('disabled');
     jQuery('#div_numerosidoc_documento').hide();
     jQuery('#tarnumsidoc_doc').attr('disabled','true');
 }
}

function abrirPopupInstituicao(tipo) {
    new Ajax.Request('ajax.php',
    {
        method: 'post',
        parameters: '',
        onComplete: function(r){
            window.open('gestaodocumentos.php?modulo=principal/popupInstituicoes&acao=A&type=' + tipo, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=500');
        }
    });
}

function validaDataRecebimento(obj) {
    var data1 = obj.value;
    var data2 = $('tardatainicio').value;

    data1 = parseInt(data1.split("/")[2].toString() + data1.split("/")[1].toString() + data1.split("/")[0].toString());
    data2 = parseInt(data2.split("/")[2].toString() + data2.split("/")[1].toString() + data2.split("/")[0].toString());

    if (data1 > data2) {
        alert('A Data do Recebimento não pode ser maior que a data de início');
        obj.value = "";
        obj.focus();
    }
}

jQuery(document).ready(function(){
    if(jQuery('#hid_tartiponumsidoc').val() == 'D'){
     jQuery('#div_numerosidoc_documento').show();
     jQuery('#tarnumsidoc_doc').removeAttr('disabled');
     jQuery('#div_numerosidoc_processo').hide();
     jQuery('#tarnumsidoc_proc').attr('disabled','true');
 } else if(jQuery('#hid_tartiponumsidoc').val() == 'P'){
     jQuery('#div_numerosidoc_processo').show();
     jQuery('#tarnumsidoc_proc').removeAttr('disabled');
     jQuery('#div_numerosidoc_documento').hide();
     jQuery('#tarnumsidoc_doc').attr('disabled','true');
 }
});

</script>

<?PHP

}

#É CHAMADO NA TELA CADASTRO DE DEMANDAS - MONTA O PARTE DO FORMULARIO DE DEMANDA, TAREFA E ATENDIMENTO.
function blocoDadosAtendimento(&$obTarefa, $boMensagemObrig = 'N', $boCadAcompanhamento = false, $boCadTarefa = false, $acodsc = '', $arCadTarefa = array(), $boCadAtividade = false, $db = false) {
    if (!$db) {
        global $db;
    }

    $tarid = $obTarefa->tarid;

    $permissao = buscaPermissaoPerfilSalvar( $tarid );

    if( $permissao == 'S' ){
        $habilitado = 'S';
        $disabled = '';
    }else{
        $habilitado = 'N';
        $disabled = 'disabled="disabled"';
    }

    if ($boCadAcompanhamento) { # se for estiver no cadastro de tarefa, não monta tabela
        ?>
        <!-- DADOS DO ATENDIMENTO -->
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <?  } ?>
            <tr>
                <td align="left" colspan="5"><b>DADOS DO ATENDIMENTO</b></td>
            </tr>
            <? if ($boCadAcompanhamento) { ?>
            <!--BLOCO DE CÓDIGO RELACIONADO A TELA DE ACOMPANHAMENTO-->
            <tr>
                <td class="SubTituloDireita" align="right" valign="top" width="300px">
                    <?PHP
                    $boAtividade = $obTarefa->boAtividade();
                    if ( $boAtividade ) {
                        echo 'Atividade';
                    } else {
                        echo 'Tarefa';
                    }
                    ?>
                </td>
                <td>
                    <?PHP
                    echo "<b>" . $obTarefa->tartitulo . "</b><br />" . $obTarefa->tardsc;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Setor de Origem:</td>
                <td colspan="3">
                    <?PHP
                        $unaidsetororigem = $obTarefa->unaidsetororigem;
                        $sql = "
                            SELECT  unaid as codigo,
                                    unadescricao as descricao
                            FROM gestaodocumentos.unidade
                            ORDER BY unadescricao
                        ";
                        $db->monta_combo("unaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', '520', 'S', 'unaidsetororigem', false, $unaidsetororigem, 'Setor de Origem');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Setor Responsável:</td>
                <td colspan="3">
                    <?PHP
                    $unaidsetorresponsavel = $obTarefa->unaidsetorresponsavel;

                    if (!$unaidsetorresponsavel){
                        $unaidsetorresponsavel = $unaidsetororigem;
                    }
                    $sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM gestaodocumentos.unidade ORDER BY unasigla";
                    $db->monta_combo("unaidsetorresponsavel", $sql, $habilitado, 'Selecione...', '', '', '', '520', 'S', 'unaidsetorresponsavel', false, $unaidsetorresponsavel, 'Setor de Responsável');
                    ?>
                    <input type="hidden" value="<?PHP echo $unaidsetorresponsavel; ?>" name="unaidsetorresponsavelAnterior" id="unaidsetorresponsavelAnterior">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Responsável pela Demanda:</td>
                <td id="td_usucpfresponsavel2" colspan="3">
                    <?PHP
                    $sql = "SELECT  	u.usucpf as codigo,
                    u.usunome as descricao
                    FROM 		seguranca.usuario AS u
                    LEFT JOIN 	seguranca.perfilusuario AS p ON p.usucpf = u.usucpf
                    WHERE 		p.pflcod IN (".PERFIL_GDOCUMENTO_APOIO .",". PERFIL_GDOCUMENTO_COORDENACAO_GERAL .",". PERFIL_GDOCUMENTO_DIRETORIA .",". PERFIL_GDOCUMENTO_ESQUIPE_TEC .",". PERFIL_GDOCUMENTO_ADMINISTRADOR .")
                    ORDER BY 	u.usunome";

                    $usucpfresponsavel = $obTarefa->usucpfresponsavel;
                    $db->monta_combo('usucpfresponsavel', $sql, $habilitado, 'Selecione...', '', '', '', '520', 'S', 'usucpfresponsavel', false, $usucpfresponsavel, 'Responsável pela Demanda' );
                    ?>
                </td>
                <input type="hidden" value="<?PHP echo $usucpfresponsavel; ?>" name="usucpfresponsavelAnterior" id="usucpfresponsavelAnterior">
            </tr>
            <tr>
                <td class="SubTituloDireita" align="right">Situação da Demanda:</td>
                <td colspan="3">
                    <?PHP
                    $sql = "
                        SELECT  sitid AS codigo,
                                sitdsc AS descricao
                        FROM gestaodocumentos.situacaotarefa
                        ORDER BY codigo
                    ";
                    $sitid = $obTarefa->sitid;
                    $tarid = $obTarefa->tarid;
                    $db->monta_combo('sitid', $sql, $habilitado, "", '', '', '', '520', 'S', 'sitid', false, $sitid, 'Situação da Demanda');
                    ?>
                    <input type="hidden" value="<?PHP echo $sitid; ?>" name="sitidAnterior" id="sitidAnterior">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" align="right">Prazo para Atendimento:</td>
                <td colspan="3">
                    <?PHP
                    $obData = new Data();
                    $tardataprazoatendimento = $obTarefa->tardataprazoatendimento;
                    if ($tardataprazoatendimento) {
                        $tardataprazoatendimentoAnterior = $obData->formataData($tardataprazoatendimento, "dd/mm/YYYY");
                    }
                    if ($arCadTarefa['tardataprazoatendimento']) {
                        $obData = new Data();
                        $tardataprazoatendimento = $obData->formataData($tardataprazoatendimento, "YYYY-mm-dd");
                    }
                    echo campo_data2('tardataprazoatendimento', 'S', $habilitado, 'Prazo para Atendimento', 'S', '', 'verificaDataPaiEDataFilha(this,'. $tarid.', 1);', $tardataprazoatendimento);
                    ?>
                    <input type="hidden" value="<?PHP echo $tardataprazoatendimentoAnterior; ?>" name="tardataprazoatendimentoAnterior" id="tardataprazoatendimentoAnterior">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Mensagem:</td>
                <td colspan="3">
                    <?PHP
                    if( $tarid ){
                        $obAcomp = new GestaoDocumentos();
                        $msg_despacho = $obAcomp->recuperaAcompanhamentoTarid( $tarid );
                    }
                    $acodsc = $msg_despacho[0]['acodsc'];
                    echo campo_textarea('acodsc', 'N', '', 'Mensagem ', 104, 5, 1500, '', 0, '', false, false, '', $acodsc);
                    ?>
                </td>
            </tr>
            <!--BLOCO DE CÓDIGO RELACIONADO A TELA DE ACOMPANHAMENTO-->
            <?  }
            if ( !$boCadAcompanhamento ) {
                ?>
                <tr>
                   <td class="SubTituloDireita">Tipo:</td>
                   <td colspan="4">
                    <?PHP
                    $sql = "
                    SELECT  tmdid AS codigo,
                    tmddescricao AS descricao
                    FROM gestaodocumentos.tipomodalidade
                    ORDER BY descricao
                    ";
                    $tmdid = $obTarefa->tmdid;
                    $db->monta_combo('tmdid', $sql, $habilitado, "Selecione...", '', '', '', '520', 'N', 'tmdid', false, $tmdid, 'Modalidade');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" align="right">Assunto:</td>
                <td colspan="4">
                    <?PHP
                    $sql = "
                        SELECT  temid AS codigo,
                                temdescricao AS descricao
                        FROM gestaodocumentos.tema
                        WHERE temstatus = 'A'
                        ORDER BY descricao
                    ";
                    $temid = $obTarefa->temid;
                    $db->monta_combo('temid', $sql, $habilitado, "Selecione...", '', '', '', '520', 'N', 'temid', false, $temid, 'Tema');
                    ?>
                </td>
            </tr>
            <?  } ?>
            <?PHP
            if (!$boCadAcompanhamento) {
                echo blocoDadosObjDemanda($obTarefa, $arCadTarefa);
            }
            ?>
            <?PHP if (!$boCadAcompanhamento) { ?>
            <tr>
                <td class="SubTituloDireita" align="right">Situação da Demanda:</td>
                <td colspan="4">
                    <?PHP
                    $sql = "
                    SELECT  sitid AS codigo,
                    sitdsc AS descricao
                    FROM gestaodocumentos.situacaotarefa order by codigo
                    ";
                    $sitid = $obTarefa->sitid;
                    $tarid = $obTarefa->tarid;
                    $db->monta_combo('sitid', $sql, $habilitado, "", '', '', '', '520', 'N', 'sitid', false, $sitid, 'Situação da Demanda');
                    ?>
                    <input type="hidden" value="<?PHP echo $sitid; ?>" name="sitidAnterior" id="sitidAnterior">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" align="right">Nivel de Complexidade:</td>
                <td colspan="4">
                    <?PHP
                    $sql = "
                    SELECT  nvcid AS codigo,
                    nvcdsc AS descricao
                    FROM gestaodocumentos.nivelcomplexidade
                    ORDER by descricao
                    ";
                    $nvcid = $obTarefa->nvcid;
                    $db->monta_combo('nvcid', $sql, $habilitado, "", '', '', '', '520', 'N', 'nvcid', false, $nvcid, 'Nivel de Complexidade' );
                    ?>
                    <input type="hidden" value="<?PHP echo $sitid; ?>" name="sitidAnterior" id="sitidAnterior">
                </td>
            </tr>
            <tr>
               <td class="SubTituloDireita">Descrição/Detalhamento:</td>
               <td colspan="4">
                <?PHP
                $tardsc = stripslashes( $obTarefa->tardsc );
                echo campo_textarea('tardsc', 'N', $habilitado, 'Descrição/Detalhamento', 104, 5, 2000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, '', $tardsc);
                ?>
            </td>
        </tr>
        <? } ?>

        <? if (!$boCadAcompanhamento) { ?>
        <tr>
            <td align="left" colspan="5"><b>DADOS DO ARQUIVAMENTO E/OU TRAMITAÇÃO</b></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Arquivamento e/ou Tramitação:</td>
            <td colspan="4">
                <?PHP
                $tarsitarquivo = stripslashes($obTarefa->tarsitarquivo);
                echo campo_textarea('tarsitarquivo', 'N', $habilitado, 'Arquivamento', 104, 3, 255, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Arquivamento', $tarsitarquivo);
                ?>
            </td>
        </tr>
        <? } ?>

        <? if ($boCadAcompanhamento) { ?>
    </table>

    <?PHP
}

}

#É CHAMADO NA TELA CADASTRO DE DEMANDAS - LISTA OS DADOS DAS SOLICITANTES CADASTRADAS A DEMANDA.
function blocoDadosSolicitante(&$obTarefa, &$arCadTarefa = null){
	global $db;

    if( !$boCadAcompanhamento ){
        if($obTarefa->tarid){
            $taridprincipal = $obTarefa->tarid;
        }else{
            $taridprincipal = 0;
        }
        ?>
        <tr>
            <td rowspan="2" class="SubTituloDireita">Solicitante:</td>
            <td colspan="4">
                <?PHP
                    $acao_a = "
                        <img align=\"absmiddle\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alteraSolicitante('||s.solid||','||tarid||')\" title=\"Alterar Solicitante\">
                        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaSolicitante('||iss.issid||')\" title=\"Deletar Solicitante\">
                    ";

                    $acao_b = "
                        <img align=\"absmiddle\" src=\"../imagens/alterar_01.gif\" style=\"cursor: pointer\" title=\"Alterar Solicitante\">
                        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaSolicitante('||iss.issid||')\" title=\"Deletar Solicitante\">
                    ";

                    $sql = "
                        SELECT  CASE WHEN s.solid IS NOT NULL
                                    THEN '$acao_a'
                                    ELSE '$acao_b'
                                END AS acao,

                                CASE
                                    WHEN iss.iesidinstituicaoensino IS NOT NULL THEN ie.iesid
                                    WHEN iss.uamid IS NOT NULL THEN u.uamid
                                    WHEN iss.ogsid IS NOT NULL THEN og.ogsid
                                    WHEN iss.solidpessoafisica IS NOT NULL THEN s.solid
                                    WHEN iss.mntid IS NOT NULL THEN m.mntid
                                END AS codigo,

                                CASE
                                    WHEN iss.iesidinstituicaoensino IS NOT NULL THEN UPPER(ie.iesdsc)
                                    WHEN iss.uamid IS NOT NULL THEN UPPER(u.uamdsc)
                                    WHEN iss.ogsid IS NOT NULL THEN UPPER(og.ogsdsc)
                                    WHEN iss.solidpessoafisica IS NOT NULL THEN UPPER(s.solnome)
                                    WHEN iss.mntid IS NOT NULL THEN UPPER(m.mntdsc)
                                END AS descricao,

                                ts.tpsdsc
                        FROM gestaodocumentos.instituicaosolicitante AS iss

                        JOIN gestaodocumentos.tiposolicitante AS ts ON ts.tpsid = iss.tpsid

                        --INSTITUIÇÃO DE ENSINO
                        LEFT JOIN (
                            SELECT  iesid,
                                    iesdsc
                            FROM gestaodocumentos.instituicaoensino
                        ) AS ie ON ie.iesid = iss.iesidinstituicaoensino

                        --ÁREA MEC
                        LEFT JOIN (
                            SELECT  uamid,
                                    uamdsc
                            FROM public.unidadeareamec
                        ) AS u  ON u.uamid = iss.uamid

                        --ÓRGÃO
                        LEFT JOIN (
                            SELECT  ogsid,
                                    ogsdsc
                            FROM gestaodocumentos.orgaosolicitante
                        )AS og ON og.ogsid = iss.ogsid

                        --SOLICITANTE
                        LEFT JOIN (
                            SELECT solid,
                                    solnome
                            FROM gestaodocumentos.solicitantepessoa
                        )AS s ON s.solid = iss.solidpessoafisica

                        --MANTENEDORA
                        LEFT JOIN (
                            SELECT  mntid,
                                    mntdsc
                            FROM gestaodocumentos.mantenedoras
                        )AS m ON m.mntid = iss.mntid

                        WHERE iss.tarid = {$taridprincipal}

                        ORDER BY descricao
                    ";
                    $cabecalho = array( "Ação", "Código" , "Solicitante", "Tipo de Solicitante"  );
                    $alinhamento = Array( 'center', 'left', 'left', 'left' );
                    $tamanho = Array( '5%', '10%', '45%', '40%' );

                    $param['totalLinhas'] = false;
                    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento, null, $param);
            ?>
        </td>
    </tr>
    <? if($taridprincipal){ ?>
        <tr>
            <td colspan="4" class="subTituloCentro">
                <input type="button" value="Adicionar Solicitante" onclick="return abrirPopupSolicitante('<?=$obTarefa->tarid;?>');">
            </td>
        </tr>
    <? }else{ ?>
        <tr>
            <td class="subTituloCentro">&nbsp;</td>
            <td colspan="3" class="subTituloCentro" align="center">SALVE A DEMANDA PARA ADICIONAR O SOLICITANTE.</td>
        </tr>
<?PHP
        }
    }
}

#É CHAMADO NA TELA CADASTRO DE DEMANDAS - LISTA OS DADOS DAS OBJETOS DE DEMANDAS CADASTRADAS A DEMANDA.
function blocoDadosObjDemanda(&$obTarefa, &$arCadTarefa = null){
	global $db;

    if( !$boCadAcompanhamento ){
        if($obTarefa->tarid){
            $taridprincipal = $obTarefa->tarid;
        }else{
            $taridprincipal = 0;
        }
?>
        <tr>
            <td rowspan="2" class="SubTituloDireita">Entidade Objeto da Demanda:</td>
            <td colspan="4">
                <?PHP
                $acao = "
                    <center>
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaObjDemanda('||od.ojdid||')\" title=\"Deletar Objeteos da Demanda\" >
                    </center>
                ";

                $sql = "
                    SELECT  '{$acao}',
                        CASE
                            WHEN od.iesid IS NOT NULL THEN ie.iesid
                            WHEN od.mntid IS NOT NULL THEN m.mntid
                            WHEN od.oniid IS NOT NULL THEN o.oniid
                        END AS codigo,

                        CASE
                            WHEN od.iesid IS NOT NULL THEN UPPER(ie.iesdsc)
                            WHEN od.mntid IS NOT NULL THEN UPPER(m.mntdsc)
                            WHEN od.oniid IS NOT NULL THEN UPPER(o.oniidsc)
                        END AS descricao,

                        CASE
                            WHEN od.ojdtipo = 'I' THEN 'IES'
                            WHEN od.ojdtipo = 'M' THEN 'MANTENEDORA'
                            WHEN od.ojdtipo = 'O' THEN 'OUTROS'
                            WHEN od.ojdtipo = 'N' THEN 'Não IES'
                            WHEN od.ojdtipo = 'D' THEN 'IES Descredenciado'
                        END AS ojdtipo

                    FROM gestaodocumentos.objetodemanda AS od

                    --INSTITUIÇÃO DE ENSINO
                    LEFT JOIN (
                        SELECT  iesid,
                                iesdsc
                        FROM gestaodocumentos.instituicaoensino
                    )AS ie ON ie.iesid = od.iesid
                    --MATENEDORA
                    LEFT JOIN (
                        SELECT 	mntid,
                        mntdsc
                        FROM gestaodocumentos.mantenedoras
                        )AS m ON m.mntid = od.mntid
                    --OUTROS
                    LEFT JOIN (
                        SELECT 	oniid,
                        oniidsc
                        FROM gestaodocumentos.objetonaoies
                        )AS o ON o.oniid = od.oniid

                    WHERE od.tarid = {$taridprincipal}

                    ORDER BY 1
                ";
                $cabecalho = array( "Ação", "Código" , "Objeto da Demanda", "Tipo de Objeto da Demanda"  );
                $alinhamento = Array( 'center', 'left', 'left', 'left' );
                //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
                $param['totalLinhas'] = false;
                $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento, null, $param);
?>
            </td>
        </tr>
    <?PHP if($taridprincipal){ ?>
            <tr>
                <td colspan="4" class="subTituloCentro">
                    <input type="button" value="Adicionar Objeto da Demanda" onclick="return abrirPopupObjetoDemandante('<?=$obTarefa->tarid;?>');">
                </td>
            </tr>
    <?PHP } else { ?>
            <tr>
                <td class="subTituloCentro">&nbsp;</td>
                <td colspan="3" class="subTituloCentro" align="center">SALVE A DEMANDA PARA ADICIONAR O OBJETO DEMANDANTE.</td>
            </tr>
    <?PHP
        }
    }
}

function cabecalhoTarefa($tarid, $db = false) {
    if (!$db) {
        global $db;
    }

    $obTarefa = new GestaoDocumentos();
    $tartarefa = $obTarefa->pegaTartarefaPorTarid($tarid);
    $obTarefa = new GestaoDocumentos($tartarefa);

    $_SESSION['dados_tarefa']['tarid'] = $obTarefa->tarid;
    $obData = new Data();
    $_SESSION['dados_tarefa']['tardataprazoatendimento'] = $obData->formataData($obTarefa->tardataprazoatendimento, "dd/mm/YYYY");
    ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tr>
    		<td align="left" colspan="4"><b>Dados da Demanda</b></td>
    	</tr>
    	<tr>
    		<td class="SubTituloDireita" width="35%">Número SIDOC:</td>
    		<td>
                <?PHP
                $tarnumsidoc = $obTarefa->tarnumsidoc;
                $tartiponumsidoc = $obTarefa->tartiponumsidoc;

                if ($tartiponumsidoc == 'D'){
                    echo substr($tarnumsidoc, 0, 6) . "/" . substr($tarnumsidoc, 6, 4) . "-" . substr($tarnumsidoc, 10, 2);
                }else{
                    echo substr($tarnumsidoc, 0, 5) . "." . substr($tarnumsidoc, 5, 6) . "/" . substr($tarnumsidoc, 11, 4) . "-" . substr($tarnumsidoc, 15, 2);
                }
                ?>
            </td>
        </tr>
        <tr>
          <td class="SubTituloDireita" width="300px">Situação Workflow:</td>
          <td><?PHP echo $obTarefa->pegaEstadoDocumento($tarid); ?></td>
      </tr>
      <tr>
          <td class="SubTituloDireita" width="300px">Código da Demanda:</td>
          <td><?PHP echo $obTarefa->tarid; ?></td>
      </tr>
      <tr>
          <td class="SubTituloDireita">Modalidade:</td>
          <td>
            <?PHP
            if ($obTarefa->tmdid) {
                $tmddescricao = $db->pegaUm("SELECT tmddescricao FROM gestaodocumentos.tipomodalidade WHERE tmdid = {$obTarefa->tmdid}");
                echo $tmddescricao;
            }
            ?>
        </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Solicitantes:</td>
      <td>
        <?PHP
        $solicitantesTemp = "";
        $arSolicitantes = $obTarefa->recuperaSolicitantesPorTarid($obTarefa->tarid);
        foreach ($arSolicitantes as $solicitantes) {
            $solicitantesTemp .= $solicitantes['solnome'] . ", ";
        }
        if ($solicitantesTemp) {
            echo substr($solicitantesTemp, 0, strlen($solicitantesTemp) - 2);
        }
        ?>
    </td>
</table>

<?PHP
}

function listaAtendimento($tarid, $db = false){
	if(!$db){
		global $db;
	}

	header('Content-Type: text/html; charset=iso-8859-1');

	$cpf = $_SESSION['usucpf'];

	$acao = "CASE WHEN atv.usucpf = '{$cpf}' THEN '<img src=\"../imagens/alterar.gif\" id=\"' || atv.atvid ||'\" class=\"alterar\" onclick=\"visualizarAtividade('|| atv.atvid ||','|| atv.tarid ||');\" style=\"cursor:pointer;\"/> '
    ELSE '' END ||
    '<img src=\"../imagens/excluir.gif\" id=\"' || atv.atvid ||'\" class=\"excluir\" onclick=\"excluirAtividade('|| atv.atvid ||','|| atv.tarid ||');\" style=\"cursor:pointer;\"/>' as acao,";

    $sql = "SELECT 		$acao
    '<textarea id=\"atvdetalhe\" class=\"obrigatorio txareanormal\" style=\"width:120ex;\" rows=\"5\" cols=\"20\" name=\"atvdetalhe\">'|| atv.atvdetalhe ||'</textarea>',
    usu.usunome,
    atv.atvhistworflow,
    to_char(atv.atvdtinclusao,'DD/MM/YYYY')
    FROM 		gestaodocumentos.atividade atv
    INNER JOIN	seguranca.usuario usu ON atv.usucpf = usu.usucpf
    WHERE		tarid = {$tarid} AND atvstatus = 'A'";
    // ver(simec_htmlentities($sql),d);

    $cabecalho = array("Ação","Detalhe", "Responsável", "Histórico Workflow", "Data" );
    $db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
    exit;
}

function boExisteTarefa( $tarid, $boMensagem = false){
	global $db;
	$tarefa = "";

	if($tarid){
		$tarefa = $db->pegaUm("SELECT tarid FROM gestaodocumentos.tarefa WHERE tarid = {$tarid}");
		if( !$tarefa && $boMensagem){
			echo "<script>
           alert('A Tarefa / Atividade informada não existe!');
           history.back(-1);
           </script>";
           die;
       } else {
         return true;
     }
 }
}

#Função que retorna o array para montar as abas do Acompanhamento
function carregaAbasAcompanhamento($pagina,$ptaid='',$pacid='') {
	global $db;

	switch($pagina) {

		case 'cadAcompanhamento':
     $menu = array(
         0 => array("id" => 1, "descricao" => "Atendimento", "link" => "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A"),
         1 => array("id" => 2, "descricao" => "Restrição",   "link" => "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadRestricao&acao=A")
         );
     break;

     case 'cadRestricao':
     $menu = array(
         0 => array("id" => 1, "descricao" => "Atendimento", "link" => "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A"),
         1 => array("id" => 2, "descricao" => "Restrição",   "link" => "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadRestricao&acao=A")
         );
     break;
 }

 $menu = $menu ? $menu : array();

 return $menu;
}

function dadosAtendimento($db = null, &$tarid){
	if(!$db){
		global $db;
	}
    ?>

    <div id="divAtendimento">
       <form id="formatendimento" name="formatendimento" action="">
        <input type="hidden" name="boAtendimento" id="boAtendimento" value="1" />
        <input type="hidden" name="atvid" id="atvid" value="" />
        <input type="hidden" name="tarid" id="tarid" value="<?PHP echo $tarid;?>" />
        <input type="hidden" name="requisicao" id="requisicao" value="cadastrar_atividade" />
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="95%">
            <tr>
                <td align="left"><b>Atividade(s)</b></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align: top;">Detalhe:</td>
                <td><?PHP echo campo_textarea('atvdetalhe', 'N', 'S', '', '100', '5', '' ); ?></td>
            </tr>
            <tr align="center"style="background-color:#cccccc">
                <td colspan="2">
                    <input type="button" name="botao" value="Salvar" onclick="gravarAtividade();" />
                </td>
            </tr>
        </table>
    </form>
    <div id="divListaAtendimento"><?PHP echo listaAtendimento($tarid, $cpf); ?></div>
</div>
<?PHP die();
}

function dadosRetricao($db = null, &$tarid){
	if(!$db){
		global $db;
	}
    ?>

    <div id="divRestricao">
        <input type="hidden" name="boRestricao" id="boRestricao" value="1" />
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="95%">
            <tr>
                <td class="SubTituloDireita" align="right" valign="top" width="300px">
                    <?PHP
                    $obTarefa = new GestaoDocumentos($tarid);
                    $boAtividade = $obTarefa->boAtividade();

                    if($boAtividade){
                        echo 'Atividade';
                    } else {
                        echo 'Tarefa';
                    }
                    ?>
                </td>
                <td>
                    <?PHP
                    echo "<b>".$obTarefa->tartitulo."</b><br />".$obTarefa->tardsc;
                    ?>
                </td>
            </tr>
            <tr>
                <td align="left"><b>Dados da Restrição</b></td>

            </tr>
            <tr>
                <td align='right' class="SubTituloDireita"
                style="vertical-align: top;" width="25%">Restrição:</td>
                <td>
                    <?PHP echo campo_textarea( 'resdescricao', 'N', 'S', 'Restrição', 70, 8, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Restrição' ); ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita"
                style="vertical-align: top;">Providência:</td>
                <td>
                    <?= campo_textarea( 'resmedida', 'N', 'S', '', 80, 3, 250 ); ?>
                </td>
            </tr>
            <tr style="background-color: #cccccc">
                <td align='right' style="vertical-align: top;">&nbsp;</td>
                <td>
                    <input type="button" name="botao" value="Salvar" onclick="gravarRestricao('', <?= $obTarefa->_tartarefa ?>)" />
                </td>
            </tr>
        </table>

        <div id="divListaRestricao">
            <?PHP echo listaRetricao($db, $tarid); ?>
        </div>
    </div>
    <?
    die();
}

function listaRetricao($db, &$tarid){
	if(!$db){
		global $db;
	}

	$obTarefa = new GestaoDocumentos($tarid);

	$sql = "
    SELECT  r.resid,
    to_char(r.resdata, 'DD/MM/YYYY') as data,
    r.resdescricao,
    r.usucpf,
    r.resmedida,
    r.ressolucao,
    u.usunome,
    u.usufoneddd as dddresponsavel,
    u.usufonenum as telefoneresponsavel,
    tu.unadescricao
    FROM gestaodocumentos.restricao AS r

    JOIN seguranca.usuario AS u ON r.usucpf = u.usucpf
    LEFT JOIN gestaodocumentos.unidadeusuario AS uu ON r.usucpf = uu.usucpf
    LEFT JOIN gestaodocumentos.unidade AS tu ON uu.unaid = tu.unaid

    WHERE r.resstatus = 'A' and tarid = {$obTarefa->tarid}
    ";
    $arRestricao = $db->carregar($sql);
    $arRestricao = ($arRestricao) ? $arRestricao : array();

    ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="95%">
        <tbody>
            <tr style="background-color: #cccccc">
                <td align='right' style="vertical-align: top; width: 25%">&nbsp;</td>
                <td align='left' style="vertical-align: top;"><b>Restrição</b>
                    <img src="../imagens/restricao.png" border="0" align="absmiddle" style="margin: 0 3px 0 3px;" />
                </td>
            </tr>
            <?PHP foreach ($arRestricao as $restricao){ ?>
            <tr>
                <td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Descrição:</td>
                <td id="celDescricao_<?= $restricao['resid'] ?>" name="celDescricao_<?= $restricao['resid'] ?>">
                    <input type="hidden" id="hiddenDesc[<?= $restricao['resid'] ?>]" name="hiddenDesc[<?= $restricao['resid'] ?>]" value="<?= $restricao['resdescricao'] ?>" />

                    <div id="divDesc1_<?= $restricao['resid'] ?>" style="display: none">
                        <textarea name="resdescricao_<?= $restricao['resid'] ?>" id="resdescricao_<?= $restricao['resid'] ?>" rows="5" cols="70" class="text_editor_simple"><?= $restricao['resdescricao'] ?></textarea>
                    </div>
                    <div id="divDesc2_<?= $restricao['resid'] ?>" style="display: ''"><?= $restricao['resdescricao'] ?> </div>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Data:</td>
                <input type="hidden" id="hiddenData[<?= $restricao['resid'] ?>]" name="hiddenData[<?= $restricao['resid'] ?>]" value="<?= $restricao['data'] ?>" />
                <td id="celData_<?= $restricao['resid'] ?>" name="celData_<?= $restricao['resid'] ?>"><?= ( $restricao['data'] ); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Autor:</td>
                <td>
                    <div>
                        <?= $restricao['usunome'] ?>
                    </div>
                    <div style="color: #959595;">
                        <?= $restricao['unadescricao'] ?> - Tel: (<?= $restricao['dddresponsavel'] ?>) <?= $restricao['telefoneresponsavel'] ?>
                    </div>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Restrição superada?</td>
                <td>
                    <label title="indica que a restrição está superada">
                        <input type="radio" name="ressolucao_<?= $restricao['resid'] ?>" value="t" <?= $restricao['ressolucao'] == 't' ? 'checked="checked"' : '' ?> />Sim
                    </label> &nbsp;&nbsp;
                    <label title="indica que a restrição não está superada">
                        <input type="radio" name="ressolucao_<?= $restricao['resid'] ?>" value="f" <?= $restricao['ressolucao'] == 'f' ? 'checked="checked"' : '' ?> /> Não
                    </label>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Providência:</td>
                <td>
                    <?PHP
                    $resmedida = $restricao["resmedida"];
                    echo campo_textarea( 'resmedida_'.$restricao['resid'], 'N', 'S', 'Providência ', 70, 2, 250, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Providência', $resmedida );
                    ?>
                </td>
            </tr>
            <tr style="background-color: #cccccc">
                <td align='right' style="vertical-align: top; width: 25%">&nbsp;</td>
                <td>
                    <?PHP
                    if( $restricao['usucpf'] == $_SESSION['usucpf']){
                        ?>

                        <input type="button" name="bntAltera_<?= $restricao['resid'] ?>" id="bntAltera_<?= $restricao['resid'] ?>" value="Alterar" onclick="alteraCampoDescricao(<?= $restricao['resid'] ?>);" />

                        <?PHP
                    }
                    ?>
                    <input type="button" name="botao" value="Salvar" onclick="gravarRestricao(<?= $restricao['resid']?>, <?= $obTarefa->_tartarefa ?>); return void(0);" />
                    <input type="button" name="botao" value="Excluir" onclick="excluirRestricao(<?= $restricao['resid']?>, <?= $obTarefa->tarid ?> ); return void(0);" />
                </td>
            </tr>
            <?PHP } ?>
        </tbody>
    </table>
    <?PHP
}

function verificaSeConcluido( $tarid ){
    $db = new cls_banco();

    $docid = $db->pegaUm("SELECT  docid FROM gestaodocumentos.tarefa WHERE tarid = {$tarid['tarid']}");
    $esdid = $db->pegaUm("SELECT  ed.esdid FROM workflow.documento d JOIN workflow.estadodocumento AS ed ON ed.esdid = d.esdid WHERE d.docid = $docid");

    if( $esdid == WF_PROCESSO_ARQUIVADO || $esdid == WF_PROCESSO_FINALIZADO ){
        $concluido = 'S';
    }else{
        $concluido = 'N';
    }

    echo '<resp>'.$concluido.'</resp>';
}


#--------------------------------------------- FUNÇÕES WORKFLOW MODULO GESTÃO DE DOCUMENTOS - CADASTRO DE DEMANDAS ----------------------------------#

#REGRAS WORKFLOW - BUSCA DOCID VERIFICA SE O DOCUENTO JÁ EXISTE.
function buscarDocidGestaoDocumentos( $tarid ){
    global $db;

    $sql = "
    SELECT  tarid,
    docid
    FROM gestaodocumentos.tarefa
    WHERE tarid = {$tarid}
    ";
    $dados = $db->pegaLinha($sql);
    return $dados['docid'];
}

#REGRAS WORKFLOW - CRIA O DOCUMENTO CASO NÃO EXISTA.
function criaDocidGestaoDocumentos( $tarid ){
    global $db;

    require_once APPRAIZ ."includes/workflow.php";

    $existeDocid = buscarDocidGestaoDocumentos( $tarid );

    if($existeDocid == ''){
        $tpdid = FUXO_GESTAO_DOCUMENTOS;

        if($tarid != ''){
            $docid = wf_cadastrarDocumento($tpdid, 'Getão de Documentos - Demandas');

            $sql = "
            UPDATE gestaodocumentos.tarefa SET docid = {$docid} WHERE tarid = {$tarid};
            ";

            if( $db->executar($sql) ){
                $db->commit();
            }else{
                $db->$db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/cadTarefa', '&acao=A');
            }
        }
    }
    return false;
}

#PEGA ESTADO ATUAL DO DOCUMENTO DO WORKFLOW.
function pegaEstadoAtualGestaoDocumentos($docid){
    global $db;

    if($docid) {
        $docid = (integer) $docid;
        $sql = "
        SELECT  ed.esdid, ed.esddsc
        FROM workflow.documento d
        JOIN workflow.estadodocumento AS ed ON ed.esdid = d.esdid
        WHERE d.docid = $docid
        ";
        $estado = $db->pegaLinha($sql);
        return $estado;
    } else {
        return false;
    }
}

#---------------------------------------------- FUNÇÕES COMPLEMENTA O WORKFLOW MODULO GESTÃO DE DOCUMENTOS ------------------------------------------#

#ENVIAR E-MAIL PARA AO USUÁRIO RESPONSAVÉL PELA DEMANDA, PODENDO ELE SER DA EQUIPE TÉCNICA OU DE APOIO. CONFORME A AÇÃO DO TRAMITE (WORKFLOW).
function enviarEmailApoioTecnico(){
    global $db;

    if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
        return true;
    } else {
        $sql = "
            SELECT  DISTINCT (u.usucpf),
                    u.usunome,
                    LOWER(u.usuemail) AS usuemail
            FROM gestaodocumentos.tarefa AS t
            JOIN seguranca.usuario AS u on u.usucpf = t.usucpfresponsavel

            WHERE t.tarid = {$_SESSION['dados_tarefa']['tarid']}

            ORDER BY 1
        ";
        $email = $db->carregar($sql);

        if(is_array($email)){
            foreach ($email as $value) {
                $arrrEmail[] = $value['usuemail'];
            }
        } else {
            $arrrEmail = array();
        }
        $e = enviarEmailTramite($arrrEmail);

        if($e){
            return true;
        } else {
            return false;
        }
    }
}

#ENVIAR E-MAIL PARA TODOS OS USUÁRIO COM O PERFIL DE COORDENAÇÃO. CONFORME A AÇÃO DO TRAMITE (WORKFLOW).
function enviarEmailCoordenacao(){
    global $db;

    atualizarPercentual();

    /*
    $docid = buscarDocidGestaoDocumentos( $_SESSION['dados_tarefa']['tarid'] );
    $verifica_estado = wf_pegarEstadoAtual( $docid );

    ver($verifica_estado, d);

    //finalizaReiteracao( $_SESSION['dados_tarefa']['tarid'] );
    */

    if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
        return true;
    } else {

        $sql = "
            SELECT  DISTINCT (u.usucpf),
                    u.usunome,
                    LOWER(u.usuemail) AS usuemail
            FROM seguranca.perfilusuario p

            JOIN seguranca.usuario u on u.usucpf = p.usucpf
            JOIN seguranca.usuario_sistema us on us.usucpf = u.usucpf AND us.sisid = {$_SESSION['sisid']}

            WHERE p.pflcod in (".PERFIL_GDOCUMENTO_COORDENACAO_GERAL.") AND us.suscod = 'A'
            ORDER BY 1
        ";
        $email = $db->carregar($sql);

        if(is_array($email)){
            foreach ($email as $value) {
                $arrrEmail[] = $value['usuemail'];
            }
        } else {
            $arrrEmail = array();
        }

        if(empty($arrrEmail)){
            return true;
        } else {
            $e = enviarEmailTramite($arrrEmail);
            if($e){
                return true;
            } else {
                return false;
            }
        }
    }
}

function enviarEmailEquipeTecnica(){
    global $db;

    atualizarMensagem();
    atualizarPercentual();

    if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
        return true;
    }else{
        $sql = "
            SELECT  DISTINCT (u.usucpf),
                    u.usunome,
                    LOWER(u.usuemail) AS usuemail
            FROM seguranca.perfilusuario p
            JOIN seguranca.usuario u on u.usucpf = p.usucpf
            JOIN seguranca.usuario_sistema us ON us.usucpf = p.usucpf AND us.sisid = {$_SESSION['sisid']}

            WHERE us.suscod = 'A' AND p.pflcod in (".PERFIL_GDOCUMENTO_ESQUIPE_TEC.") AND us.suscod = 'A'
            ORDER BY 1
        ";
        $email = $db->carregar($sql);

        if( is_array($email) ){
            foreach ($email as $value) {
               $arrrEmail[] = $value['usuemail'];
            }
        }else{
            $arrrEmail = array();
        }

        if(empty($arrrEmail)){
            return true;
        }else{
            $e = enviarEmailTramite($arrrEmail);
            if($e){
                return true;
            }else{
                return false;
            }
        }
    }
}

function enviarEmailCoordenacaoResponsavel() {
    global $db;

    atualizarMensagem();

    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        return true;
    } else {
        $sql = "
            SELECT  DISTINCT (u.usucpf),
                    u.usunome,
                    LOWER(u.usuemail) AS usuemail
            FROM gestaodocumentos.tarefa AS t

            JOIN seguranca.usuario AS u on u.usucpf = t.usucpfresponsavel
            JOIN seguranca.perfilusuario p on u.usucpf = p.usucpf

            WHERE t.tarid = {$_SESSION['dados_tarefa']['tarid']} OR p.pflcod IN (" . PERFIL_GDOCUMENTO_COORDENACAO_GERAL . ")

            ORDER BY 1
        ";
        $email = $db->carregar($sql);

        if (is_array($email)) {
            foreach ($email as $value) {
                $arrrEmail[] = $value['usuemail'];
            }
        } else {
            $arrrEmail = array();
        }
        $e = enviarEmailTramite($arrrEmail);

        if ($e) {
            return true;
        } else {
            return false;
        }
    }
}

function enviarEmailEquipeGeral($tarid) {
    global $db;

    atualizarPercentual();

    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        return true;
    } else {
        $sql = "
            SELECT  usucpfresponsavel
            FROM gestaodocumentos.tarefa
            WHERE tarid = {$tarid}
        ";
        $resp = $db->pegaUm($sql);

        if ($resp) {
            $aryWhere[] = "u.usucpf = '{$resp}'";
        }

        $aryWhere[] = "p.pflcod in (" . PERFIL_GDOCUMENTO_DIRETORIA . "," . PERFIL_GDOCUMENTO_ESQUIPE_TEC . "," . PERFIL_GDOCUMENTO_ADMINISTRADOR . "," . PERFIL_GDOCUMENTO_COORDENACAO_GERAL . ")";

        $sql = "
            SELECT  DISTINCT (u.usucpf),
                    u.usunome,
                    LOWER(u.usuemail) AS usuemail
            FROM seguranca.perfilusuario p
            JOIN seguranca.usuario u on u.usucpf = p.usucpf
            JOIN seguranca.usuario_sistema us ON us.usucpf = p.usucpf AND us.sisid = {$_SESSION['sisid']} us.suscod = 'A'

            " . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "

            ORDER BY 1
        ";
        $email = $db->carregar($sql);

        if (is_array($email)) {
            foreach ($email as $value) {
                $arrrEmail[] = $value['usuemail'];
            }
        } else {
            $arrrEmail = array();
        }

        if (empty($arrrEmail)) {
            return true;
        } else {
            $e = enviarEmailTramite($arrrEmail);
            if ($e) {
                return true;
            } else {
                return false;
            }
        }
    }
}

#ENVIAR E-MAIL PARA OS USUÁRIOS RELACIONADOS AO SISTEMA GESTÃO DE DOCUMENTOS EM TODOS OS MOMENTOS DO TRAMITE (WORKFLOW).
function enviarEmailTramite($arrDestinatarios){
    global $db;

    $sql_c = "
        SELECT	t.tartitulo,
                u.usunome,
                to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') AS tardataprazoatendimento,
                CASE
                    WHEN t.tarprioridade = 'N' THEN 'Normal'
                    WHEN t.tarprioridade = 'U' THEN 'Urgerncia'
                    WHEN t.tarprioridade = 'O' THEN 'Urgente c/ prazo de outras autoridades'
                    WHEN t.tarprioridade = 'J' THEN 'Urgente c/ prazo judicial'
                END AS tarprioridade

        FROM gestaodocumentos.tarefa t
        INNER JOIN seguranca.usuario u on u.usucpf = t.usucpfresponsavel

        WHERE t.tarid = {$_SESSION['dados_tarefa']['tarid']}
    ";

    $dadosDemanda = $db->pegaLinha($sql_c);

    $docid = buscarDocidGestaoDocumentos( $_SESSION['dados_tarefa']['tarid'] );

    $atual = wf_pegarEstadoAtual( $docid );

    $historico = wf_pegarHistorico( $docid );

    foreach ( $historico as $item ){
        $tramite .=  '<p> - '.'Origem: '.$item['esddsc'].'<br> - Destino: '.$item['aeddscrealizada'].'<br> - Feito por: '.$item['usunome'].'<br> - Na data: '.$item['htddata']. '</p>';
        $tramite .= '<hr>';
    }
    $tramite .= 'Estado atual: '.$atual['esddsc'];

    $remetente = array("nome" => "Sistema de Gestão de Documentos", "email" => "gestaodocumentos@mec.gov.br");
    $destinatario = $arrDestinatarios;
    $assunto = "Tramitação de Processo - Gestão de Documentos - SIMEC";
    $conteudo = "
        <b>Tramitação de Processo - Gestão de Documentos - SIMEC</b>
        <p>
        Titulo da Demanda: {$dadosDemanda['tartitulo']} <br>
        Responsavél: {$dadosDemanda['usunome']} <br>
        Prazo para atendimento: {$dadosDemanda['tardataprazoatendimento']} <br>
        Prioridade: {$dadosDemanda['tarprioridade']} <br>
        </p>
        {$tramite}
    ";
    $enviado = enviar_email( $remetente, $destinatario, $assunto, $conteudo );

    if( $enviado == TRUE ){
        return true;
    }else{
        return false;
    }
}

function wf_finalizaExecucaoDemanda(){
    global $db;

    $tarid = $_SESSION['dados_tarefa']['tarid'];
    $sitid = 5;

    $sql = "
    UPDATE gestaodocumentos.tarefa
    SET tarporcentoexec = 100,
    sitid = {$sitid}
    WHERE tarid = {$tarid} RETURNING tarid;";
    $dados = $db->pegaUm($sql);

    if( $dados > 0 ){
        $obTarefa = new GestaoDocumentos($tarid);

        $sitidAnt = $db->pegaUm("SELECT sitid FROM gestaodocumentos.tarefa WHERE tarid = {$tarid}");

        $_POST['acodsc'] 		= "Tramitação realizada e Demanda Concluida.";
        $_POST['sitidAnterior'] = $sitidAnt;
        $_POST['sitid'] 		= 5;

        $retorno = $obTarefa->salvarAcompanhamentoPelaArvore($_POST, 'situacao');
        $retorno = $db->commit();
    }
    return true;
}

function pesquisarTarefa($post = null) {
    header('Content-Type: text/html; charset=iso-8859-1');
    global $db;

    unset( $_SESSION['GD_filtro'] );

    $perfil = pegaPerfilGeral();

    /*if( in_array(PERFIL_GDOCUMENTO_ESQUIPE_TEC, $perfil) ){
        $aryWhere[] = "t.usucpfresponsavel = '{$_SESSION['usucpf']}'";
    }*/

    if ($post) {
        extract($post);
    }

    if($filtrotarid){
        $aryWhere[] =  "t._tartarefa = '{$filtrotarid}'";
        $_SESSION['GD_filtro']['filtrotarid'] = $filtrotarid;
    }

    if($filtrotartitulo){
        $filtrotartitulo = utf8_decode($filtrotartitulo);
        $aryWhere[] = "t.tartitulo ILIKE '%{$filtrotartitulo}%'";
        $_SESSION['GD_filtro']['filtrotartitulo'] = $filtrotartitulo;
    }

    if($filtrounaidsetororigem){
        $aryWhere[] = "t.unaidsetororigem = {$filtrounaidsetororigem}";
        $_SESSION['GD_filtro']['filtrounaidsetororigem'] = $filtrounaidsetororigem;
    }

    if($filtrousucpfresponsavel){
        $aryWhere[] = "t.usucpfresponsavel = '{$filtrousucpfresponsavel}'";
        $_SESSION['GD_filtro']['filtrousucpfresponsavel'] = $filtrousucpfresponsavel;
    }

    if($filtrosidoc){
        $aryWhere[] = "t.tarnumsidoc = '".str_replace( "-","", str_replace(".","", str_replace("/","",$filtrosidoc) ) )."'";
        $_SESSION['GD_filtro']['filtrosidoc'] = $filtrosidoc;
    }

    if($filtrotpeid){
        $aryWhere[] = "t.tpeid = {$filtrotpeid}";
        $_SESSION['GD_filtro']['filtrotpeid'] = $filtrotpeid;
    }

    if($filtroprazoini && $filtroprazofim){
        $aryWhere[] = "t.tardataprazoatendimento BETWEEN to_date('$filtroprazoini','dd/mm/yyyy') AND to_date('$filtroprazofim','dd/mm/yyyy')";
        $_SESSION['GD_filtro']['filtroprazoini'] = $filtroprazoini;
        $_SESSION['GD_filtro']['filtroprazofim'] = $filtroprazofim;
    } else if($filtroprazoini && !$filtroprazofim){
        $aryWhere[] = "t.tardataprazoatendimento >= to_date('$filtroprazoini','dd/mm/yyyy')";
        $_SESSION['GD_filtro']['filtroprazoini'] = $filtroprazoini;
    } else if(!$filtroprazoini && $filtroprazofim){
        $aryWhere[] = "t.tardataprazoatendimento <= to_date('$filtroprazofim','dd/mm/yyyy')";
        $_SESSION['GD_filtro']['filtroprazofim'] = $filtroprazofim;
    }


    if( $filtrotardtinclusao_1 && $filtrotardtinclusao_2 ){
        $aryWhere[] = "to_char(t.tardtinclusao, 'DD/MM/YYYY') BETWEEN '{$filtrotardtinclusao_1}' AND '{$filtrotardtinclusao_2}'";
        $_SESSION['GD_filtro']['filtrotardtinclusao_1'] = $filtrotardtinclusao_1;
        $_SESSION['GD_filtro']['filtrotardtinclusao_2'] = $filtrotardtinclusao_2;
    }elseif( !$filtrotardtinclusao_1 && $filtrotardtinclusao_2 ){
        $aryWhere[] = "to_char(t.tardtinclusao, 'DD/MM/YYYY') <= '{$filtrotardtinclusao_2}'";
        $_SESSION['GD_filtro']['filtrotardtinclusao_2'] = $filtrotardtinclusao_2;
    }elseif( $filtrotardtinclusao_1 && !$filtrotardtinclusao_2 ){
        $aryWhere[] = "to_char(t.tardtinclusao, 'DD/MM/YYYY') >= '{$filtrotardtinclusao_1}'";
        $_SESSION['GD_filtro']['filtrotardtinclusao_1'] = $filtrotardtinclusao_1;
    }

    if($filtrosituacao){
        $situacao = implode(",", $filtrosituacao);
        $aryWhere[] = "t.sitid IN ({$situacao})";
        $_SESSION['GD_filtro']['filtrosituacao'] = $filtrosituacao;
    }

    if($filtrotemid){
        $aryWhere[] = "t.temid = {$filtrotemid}";
        $_SESSION['GD_filtro']['filtrosituacao'] = $filtrosituacao;
    }

    if($filtrotmdid){
        $aryWhere[] = "t.tmdid = {$filtrotmdid}";
        $_SESSION['GD_filtro']['filtrotmdid'] = $filtrotmdid;
    }

    if($filtroexpressaochave){
        $filtroexpressaochave = utf8_decode($filtroexpressaochave);
        $aryWhere[] = "p.plcdsc ILIKE '%{$filtroexpressaochave}%'";
        $_SESSION['GD_filtro']['filtroexpressaochave'] = $filtroexpressaochave;
    }

    if($filtrostatusworkflow){
        $aryWhere[] = "ed.esdid = {$filtrostatusworkflow}";
        $_SESSION['GD_filtro']['filtrostatusworkflow'] = $filtrostatusworkflow;
    }

    if($filtrotiposolicitante){
        $aryWhere[] = "sol_tipo.tpsid = {$filtrotiposolicitante}";
        $_SESSION['GD_filtro']['filtrotiposolicitante'] = $filtrotiposolicitante;
    }
    
    #filtroorgao
    if($filtroorgao){
        $aryWhere[] = "sol_tipo.ogsid = {$filtroorgao}";
        $_SESSION['GD_filtro']['filtroorgao'] = $filtroorgao;
    }

    $acao = "
        <img src=\"../imagens/alterar.gif\" title=\"Alterar Tarefa\" style=\"border:0; cursor:pointer;\" onclick=\"window.location.href=\'gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid='|| t.tarid ||'\'\">
        <img src=\"../imagens/excluir.gif\" title=\"Excluir Tarefa\" style=\"border:0; cursor:pointer;\" onClick=\"excluirTarefaAtividade('|| t.tarid || ');\" >
    ";

    $rest = "
        &nbsp;<img src=\"../imagens/restricao.png\" onclick=\"window.location.href=\'gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A&tarid='|| t.tarid ||'&tartarefa='|| t._tartarefa ||'&tarpai='|| t.tarid ||'&boPadraoRetricao=1\'\" style=\"border:0; cursor:pointer;\" title=\"Restrição\">
    ";
    $anexo = "
        &nbsp;<img src=\"../imagens/anexo.gif\" onclick=\"window.location.href=\'gestaodocumentos.php?modulo=principal/cadDocumento&acao=A&tarid='|| t.tarid ||'\'\" style=\"border:0; cursor:pointer;\" title=\"Anexo\">
    ";

    $depen = "
        &nbsp;<img src=\"../imagens/botao_de.png\" title=\"Dependência Externa\">
    ";

    $reite = "
        &nbsp;<img src=\"../imagens/restricao_ico.png\" title=\"Reiteração\">
    ";

    $sql = "
        SELECT  DISTINCT '{$acao}' ||
                CASE WHEN res.qtdres > 0
                    THEN '{$rest}'
                    ELSE ''
                END ||
                CASE WHEN anx.qtdanexo > 0
                    THEN '{$anexo}'
                    ELSE ''
                END ||
                CASE WHEN t.tardepexterna IS NOT NULL
                    THEN '$depen'
                    ELSE ''
                END ||
                CASE WHEN r.taridsecundario IS NOT NULL
                    THEN '$reite'
                    ELSE ''
                END  AS acao,

                CASE WHEN t.tarprioridade = 'U' THEN '<img src=\"../imagens/pd_urgente.JPG\"/>'
                     WHEN t.tarprioridade = 'A' THEN '<img src=\"../imagens/pd_alta.JPG\"/>'
                     ELSE '<img src=\"../imagens/pd_normal.JPG\"/>'
                END as tarprioridade,

                CASE WHEN char_length(trim(tarnumsidoc)) = 12 THEN  substr(tarnumsidoc,0,7) || '.' || substr(tarnumsidoc,7,4) || '-' || substr(tarnumsidoc,11,2)
                     WHEN char_length(trim(tarnumsidoc)) = 17 THEN substring(tarnumsidoc,0,6) || '.' || substr(tarnumsidoc,6,6) || '/' || substr(tarnumsidoc,12,4) || '-' || substr(tarnumsidoc,16,2)
                     ELSE ''
                END AS sidoc,

                '<a href=\"#\" onclick=\"window.location.href=\'gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid='|| t.tarid ||'\'\"><b>' || t.tartitulo || '</b></a>' AS titulo,

                '<span style=\"color:#1E90FF;\">'|| sol.solicitante ||'</span>' AS solicitante,

                '<span style=\"color:#1E90FF;\">'||n.nvcdsc||'</span>' AS nivel,


                CASE WHEN u.usunome IS NULL
                    THEN '<span style=\"color:#1E90FF;\">Usuário Indefinido</span>'
                    ELSE '<span style=\"color:#1E90FF;\">'||u.usunome||'</span>'
                END AS nome,

                '<span style=\"color:#000000;\"><b>' || to_char(t.tardtinclusao, 'DD/MM/YYYY') || '</b></span>' AS tar_dt_inclusao,

                CASE WHEN (ed.esdid = ".WF_PROCESSO_ARQUIVADO." OR ed.esdid = ".WF_PROCESSO_FINALIZADO.")
                        THEN '<span style=\"color:#000000;\"><b>' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || '</b></span>'
                     WHEN t.tardataprazoatendimento <= CURRENT_DATE
                        THEN '<span style=\"color:#FF2020;\"><b>' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || '</b></span>'
                     WHEN ((t.tardataprazoatendimento + '1 month'::INTERVAL) >= (CURRENT_DATE + '1 month'::INTERVAL) AND  (t.tardataprazoatendimento + '1 month'::INTERVAL) <= ((CURRENT_DATE + '1 month'::INTERVAL) + '4 day'::INTERVAL))
                        THEN '<span style=\"color:#FFA500;\"><b>' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || '</b></span>'
                    ELSE '<span style=\"color:#008000;\"><b>' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || '</b></span>'
                END AS prazo_atendimento,

                CASE WHEN ed.esdid = ".WF_PROCESSO_ARQUIVADO."
                    --THEN '<span style=\"color:#1E90FF;\">Arquivado</span>
                    THEN
                        CASE WHEN di.data_inicio IS NOT NULL
                            THEN
                                CASE WHEN df.data_fim IS NOT NULL
                                    THEN
                                        CASE WHEN CAST(di.data_inicio AS DATE) = CAST(df.data_fim AS DATE)
                                            --THEN '<span style=\"color:#1E90FF;\"> 1 Dia(s) </span>'
                                            --ELSE '<span style=\"color:#1E90FF;\">'||extract(year from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(df.data_fim::DATE, di.data_inicio::DATE) ) ||' Dia(s)</span>'
                                            THEN 1
                                            ELSE extract(year from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(df.data_fim::DATE, di.data_inicio::DATE) )
                                        END
                                    ELSE
                                        CASE WHEN (extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE)) ) = 0
                                            --THEN '<span style=\"color:#1E90FF;\">'|| 1 ||' Dia(s)</span>'
                                            --ELSE '<span style=\"color:#1E90FF;\">'|| extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE)) ||' Dia(s)</span>'
                                            THEN 1
                                            ELSE extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE))
                                        END
                                END
                            --ELSE '<span style=\"color:#1E90FF;\"> 1 Dia(s) </span>'
                            ELSE 1
                        END
                    ELSE
                        CASE WHEN di.data_inicio IS NOT NULL
                            THEN
                                CASE WHEN df.data_fim IS NOT NULL
                                    --THEN '<span style=\"color:#1E90FF;\">'||extract(year from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(df.data_fim::DATE, di.data_inicio::DATE) ) ||' Dia(s)</span>'
                                    /*ELSE
                                        CASE WHEN (extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE)) ) = 0
                                            THEN '<span style=\"color:#1E90FF;\">'|| 1 ||' Dia(s)</span>'
                                            ELSE '<span style=\"color:#1E90FF;\">'|| extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE)) ||' Dia(s)</span>'
                                        END*/
                                    THEN extract(year from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(df.data_fim::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(df.data_fim::DATE, di.data_inicio::DATE) )
                                    ELSE
                                        CASE WHEN (extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE)) ) = 0
                                            THEN 1
                                            ELSE extract(year from age(NOW()::DATE, di.data_inicio::DATE) ) * 365 + extract(month from age(NOW()::DATE, di.data_inicio::DATE) ) * 30 + extract(day from age(NOW()::DATE, di.data_inicio::DATE))
                                        END
                                END
                            --ELSE '<span style=\"color:#1E90FF;\"> 1 Dia(s) </span>'
                            ELSE 1
                        END
                END AS dias_decorridos,

                sit.tarporcentoexec || '%' AS situacao,

                '<span onclick=\"exibirHistorico('|| t.docid || ');\" style=\"cursor: pointer; color:#4682B4;\" ><b>'|| ed.esddsc ||'</b></span>' as workflow

                --|| '<!-- usado para criar uma 'sub' linha relacionado com o registro 'pai' </td></tr><tr style=\"display:none\" id=\"listaTarefa_' || t.tarid || '\" ><td id=\"trV_' || t.tarid || '\" colspan=10 ></td></tr> -->' as workflow

        FROM gestaodocumentos.tarefa t
        LEFT JOIN gestaodocumentos.palavrachave p ON p.tarid = t.tarid
        LEFT JOIN seguranca.usuario u on t.usucpfresponsavel = u.usucpf
        LEFT JOIN gestaodocumentos.unidade tu ON t.unaidsetorresponsavel = tu.unaid
        LEFT JOIN gestaodocumentos.nivelcomplexidade AS n ON n.nvcid = t.nvcid
        LEFT JOIN workflow.documento d ON d.docid = t.docid
        LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
        LEFT JOIN workflow.tipodocumento td ON td.tpdid = ed.tpdid AND td.sisid = {$_SESSION['sisid']}

        LEFT JOIN gestaodocumentos.reiteracoes AS r ON r.taridprincipal = t.tarid

        LEFT JOIN gestaodocumentos.instituicaosolicitante AS sol_tipo ON sol_tipo.tarid = t.tarid

        LEFT JOIN(
            SELECT min(htddata) as data_inicio, d.docid
            FROM workflow.documento d
            INNER JOIN workflow.historicodocumento h on h.docid = d.docid
            INNER JOIN workflow.acaoestadodoc a on a.aedid = h.aedid

            WHERE esdiddestino = ".WF_EM_PREENCHIMENTO_TCENICOS."
            GROUP BY d.docid
        ) AS di ON di.docid = t.docid

        LEFT JOIN(
            SELECT min(htddata) as data_fim, d.docid
            FROM workflow.documento d
            INNER JOIN workflow.historicodocumento h on h.docid = d.docid
            INNER JOIN workflow.acaoestadodoc a on a.aedid = h.aedid
            WHERE esdiddestino = ".WF_PROCESSO_ARQUIVADO."
            GROUP BY d.docid
        ) AS df ON df.docid = t.docid

        LEFT JOIN(
            SELECT d.tarid, sd.sitid, sd.sitdsc, d.tarporcentoexec
            FROM gestaodocumentos.tarefa d
            INNER JOIN gestaodocumentos.situacaotarefa sd ON d.sitid = sd.sitid
        ) AS sit ON sit.tarid = t.tarid

        LEFT JOIN(
            SELECT COUNT(arqid) as qtdanexo, tarid
            FROM gestaodocumentos.anexo
            WHERE arqid IS NOT NULL
            GROUP BY tarid
        ) AS anx ON anx.tarid = t.tarid

        LEFT JOIN(
            SELECT COUNT(resid) as qtdres, tarid
            FROM gestaodocumentos.restricao
            WHERE ressolucao = false GROUP BY tarid
        ) AS res ON res.tarid = t.tarid

        LEFT JOIN(
            SELECT  tarid,
                    TRIM(array_to_string(
                        array(
                            SELECT  CASE
                                        WHEN ts.tpsid = 1 THEN '- ' || ts.tpsdsc --|| ':' || SUM(s.qtdsol)
                                        WHEN ts.tpsid = 2 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                        WHEN ts.tpsid = 3 THEN '- ' || ts.tpsdsc || ':<br>' || ie.iesdsc
                                        WHEN ts.tpsid = 4 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                        WHEN ts.tpsid = 5 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                        WHEN ts.tpsid = 6 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                        WHEN ts.tpsid = 7 THEN '- ' || ts.tpsdsc || ':<br>' || u.uamdsc
                                        WHEN ts.tpsid = 8 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                        WHEN ts.tpsid = 9 THEN '- ' || ts.tpsdsc || ':<br>' || og.ogsdsc
                                    ELSE ''
                                END AS descricao
                            FROM gestaodocumentos.instituicaosolicitante AS iss

                            JOIN gestaodocumentos.tiposolicitante AS ts ON ts.tpsid = iss.tpsid
                            --INSTITUIÇÃO DE ENSINO
                            LEFT JOIN (
                                SELECT  iesid,
                                        iesdsc
                                FROM gestaodocumentos.instituicaoensino
                                GROUP BY iesid, iesdsc
                            ) AS ie ON ie.iesid = iss.iesidinstituicaoensino
                            --ÁREA MEC
                            LEFT JOIN (
                                SELECT  uamid,
                                        uamdsc
                                FROM public.unidadeareamec
                                GROUP BY uamid, uamdsc
                            ) AS u  ON u.uamid = iss.uamid
                            --ÓRGÃO
                            LEFT JOIN (
                                SELECT  ogsid,
                                        ogsdsc
                                FROM gestaodocumentos.orgaosolicitante
                                GROUP BY ogsid, ogsdsc
                            ) AS og ON og.ogsid = iss.ogsid
                            --SOLICITANTE
                            LEFT JOIN (
                                SELECT  COUNT(solid) AS qtdsol,
                                        solid
                                FROM gestaodocumentos.solicitantepessoa
                                GROUP BY solid
                            ) AS s ON s.solid = iss.solidpessoafisica

                            WHERE iss.tarid = tt.tarid
                            GROUP BY ts.tpsid, ts.tpsdsc, og.ogsdsc, ie.iesdsc, u.uamdsc
                        ), '<br>'
                    ) ) AS solicitante
            FROM gestaodocumentos.tarefa tt
        ) as sol On sol.tarid = t.tarid

        ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."

        ORDER BY tar_dt_inclusao ASC

    ";//ver($sql, d);
    $alinhamento = array('center','center','','','','center','center','center','center','center', 'center', 'center');
    $tamanho = array('4%','2%','4%','25%','15%','2%','10%','4%','4%','4%','4%','4%');
    $cabecalho = array('Ação', 'Prior.', 'Nº do Documento','Identificação da Demanda', 'Tipo de Solicitante', 'Nível', 'Responsável', 'Data de Inclusão', 'Prazo de Atendimento','Dias Decorridos', 'Percentual executado', 'Status');

    $param['ordena']        = true;
    $param['totalLinhas']   = true;
    $param['managerOrder']  = array(
        9 => array( 'campo' => 't.tardtinclusao', 'alias' => 'dt_inclusao'),
        8 => array( 'campo' => 't.tardataprazoatendimento', 'alias' => 'prazo_atendimento')
    );
    $db->monta_lista($sql, $cabecalho, '50','10', null, 'center', '', '', $tamanho, $alinhamento, null, $param);
}

#CARREGA A SUB LISTA DP GRID PRINCIPAL. LISTA COMENTADA, POS FUNÇÃO ESTA SEM USO. A PEDIDO DOS USUÁRIOS DO SISTEMA.
/*
function carregarDadosTarefa($post){
    global $db;
    extract($post);

    $sql = "SELECT 		'<textarea id=\"atvdetalhe\" class=\"obrigatorio txareanormal\" style=\"width:120ex;\" rows=\"5\" cols=\"20\" name=\"atvdetalhe\">'|| atv.atvdetalhe ||'</textarea>',
    usu.usunome,
    atv.atvhistworflow,
    to_char(atv.atvdtinclusao,'DD/MM/YYYY')
    FROM 		gestaodocumentos.atividade atv
    INNER JOIN	seguranca.usuario usu ON atv.usucpf = usu.usucpf
    WHERE		tarid = {$tarid}";

    $cabecalho = array("Detalhe", "Responsável", "Histórico Workflow", "Data" );
    $db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
}
*/

function atualizarPercentual(){
    global $db;

    $sql = "
        SELECT  e.esdid
        FROM gestaodocumentos.tarefa t
        INNER JOIN workflow.documento d ON d.docid = t.docid
        INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
        WHERE t.tarid = {$_SESSION['dados_tarefa']['tarid']}
    ";
    $estado = $db->pegaUm($sql);

    if($estado){
        $sql = "
            SELECT pexvalor
            FROM gestaodocumentos.percexecutado
            WHERE esdid = {$estado}
        ";
        $valor = $db->pegaUm($sql);
    }

    if($valor){
        $sql = "
            UPDATE gestaodocumentos.tarefa
                SET tarporcentoexec = {$valor}
            WHERE tarid = {$_SESSION['dados_tarefa']['tarid']} RETURNING tarid;
        ";
        $tarid = $db->pegaUm($sql);
        $db->commit();
    }

    if($tarid > 0 ){
        return true;
    }else{
        return false;
    }
}

function pegarPerfil($usucpf){
	global $db;

	$sql = "SELECT          pu.pflcod
    FROM            seguranca.perfilusuario pu
    INNER JOIN      seguranca.perfil p ON p.pflcod = pu.pflcod
    AND             pu.usucpf = '{$usucpf}'
    AND             p.sisid = {$_SESSION['sisid']}
    AND             pflstatus = 'A'";

    $arrPflcod = $db->carregar($sql);
    !$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
    $arrPerfil = array();
    foreach($arrPflcod as $pflcod){
      $arrPerfil[] = $pflcod['pflcod'];
  }
  return $arrPerfil;
}

function atualizarMensagem(){
    global $db;
    $sql = "
        UPDATE gestaodocumentos.tarefa AS t
            SET	tarrecebimento = 't'
        WHERE tarid = {$_SESSION['dados_tarefa']['tarid']}
    ";
    $db->executar($sql);
    $db->commit();
}

#MONTA DO GRID COM ALISTAGEM DE DEMANDA RECEBIDAS OU COM O NÍVEL DE COMPLEXIDADE ALTERADO, É USADO NA TELA DE LISTAGEM DE DEMANDAS E LISTA AS DEMANDAS RELACIONADAS COM O USUÁRIO.
function monta_grid_demanda( $arrDados, $id ){
    echo "<table id=\"{$id}\" width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" class=\"listagem\">";
        echo "<thead>";
            echo "<tr>";
                echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Nº SIDOC </td>";
                echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Identificação </td>";
                echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Data de Inclusão </td>";
                echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Status </td>";
                echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Nível </td>";
            echo "</tr>";
        echo "</thead>";

        if($arrDados != '' ){
            $cont = 0;
            foreach($arrDados as $dados){
                if( $cont%2 == 0 ){
                    $bgcolor = "";
                }else{
                    $bgcolor = "#E8E8E8";
                }
                echo "<tr bgcolor=\"{$bgcolor}\">";
                    echo "<td>{$dados['tarnumsidoc']}</td>";
                    echo "<td style=\"text-align:justify; color: #4682B4;\">{$dados['tartitulo']}</td>";
                    echo "<td style=\"text-align:center; font-weight: bold;\">{$dados['tardtinclusao']}</td>";
                    echo "<td style=\"text-align:center; font-weight: bold;\">{$dados['tarstatus']}</td>";
                    echo "<td style=\"text-align:center; font-weight: bold;\">{$dados['nvcid']}</td>";
                echo "</tr>";

                $cont = $cont + 1;
            }

            $totalRegistro = $cont;
            echo "</table>";

            #DESENA A LINHA DE TOTAL NA PAGINAÇÃO.
            echo "<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" class=\"listagem\">";
                echo "<tr bgcolor=\"#ffffff\">";
                    echo "<td><b>Total de Registros: {$totalRegistro}</b></td>";
                echo "<tr>";

        }else{
            echo '<tr><td align="center" colspan="12" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
        }
        echo "</table>";
}

function buscaPermissaoPerfilSalvar( $tarid ){
    global $db;

    if( $tarid != '' ){
        $sql = "SELECT tarid FROM gestaodocumentos.tarefa WHERE tarid = {$tarid} AND usucpfresponsavel = '{$_SESSION['usucpf']}' ";
        $tarid = $db->pegaUm($sql);

        $perfil = pegaPerfilGeral();

        if( !in_array( PERFIL_GDOCUMENTO_SUPER_USUARIO, $perfil ) || in_array( PERFIL_GDOCUMENTO_ADMINISTRADOR, $perfil ) ){

            if( in_array( PERFIL_GDOCUMENTO_ESQUIPE_TEC, $perfil ) ){

                if( $tarid > 0 ){
                    $resposta = 'S';
                }else{
                    $resposta = 'N';
                }

            }else{
                $resposta = 'S';
            }

        }else{
            $resposta = 'S';
        }
    }else{
        $resposta = 'S';
    }

    return $resposta;
}

function finalizaReiteracao(){
    global $db;

    atualizarPercentual();
    
    $tarid = $_SESSION['dados_tarefa']['tarid'];

    #BUSCA DADOS - DEMANDAS E REITERAÇÕES.
    if( $tarid != '' ){
        #BUSCA OS DADOS DA DEMANDA.
        $sql = "
            SELECT tmdid, temid, sitid, nvcid, tardsc, tarsitarquivo
            FROM gestaodocumentos.tarefa
            WHERE tarid = {$tarid}
        ";
        $dados_tarefa = $db->pegaLinha($sql);
        
        $tmdid          = $dados_tarefa['tmdid'] = $dados_tarefa['tmdid'] != '' ? $dados_tarefa['tmdid'] : 'NULL';
        $temid          = $dados_tarefa['temid'] = $dados_tarefa['temid'] != '' ? $dados_tarefa['temid'] : 'NULL';
        $sitid          = $dados_tarefa['sitid'] = $dados_tarefa['sitid'] != '' ? $dados_tarefa['sitid'] : 'NULL';
        $nvcid          = $dados_tarefa['nvcid'] = $dados_tarefa['nvcid'] != '' ? $dados_tarefa['nvcid'] : 'NULL';
        $tardsc         = $dados_tarefa['tardsc'] = $dados_tarefa['tardsc'] != '' ? $dados_tarefa['tardsc'] : 'NULL' ;
        $tarsitarquivo  = $dados_tarefa['tarsitarquivo'] = $dados_tarefa['tarsitarquivo'] != '' ? $dados_tarefa['tarsitarquivo'] : 'NULL';
        
        #BUSCA AS REITERAÇÕES DA DEMANDA.
        $sql = "
            SELECT  t.tarid
            FROM gestaodocumentos.reiteracoes r
            LEFT JOIN gestaodocumentos.tarefa t ON t.tarid = r.taridsecundario
            WHERE r.taridprincipal = {$tarid}
        ";
        $reiteracoes = $db->carregarColuna($sql);
        $reiter_impl = implode(',', $reiteracoes);
    }

    #ATUALIZA AS REINTEIRAÇÕES DA RESPECTIVA DEMANDA.
    if($reiter_impl != '' ){
        $sql = "
            UPDATE gestaodocumentos.tarefa
                SET tmdid   = {$tmdid},
                    temid   = {$temid},
                    sitid   = {$sitid},
                    nvcid   = {$nvcid},
                    tardsc  = '{$tardsc}',
                    tarsitarquivo = '{$tarsitarquivo}'
            WHERE tarid IN ({$reiter_impl}) RETURNING tarid;
        ";
        $reiter_atualizada = $db->pegaUm($sql);
    }

    #ATUALIZA O OBJETO DEMANDA AS REITERAÇÕES.
    if( $reiter_atualizada > 0 ){
        #BUSCA OS DADOS DO OBEJTO DA DEMANDA.
        $sql = "
            SELECT  iesid, mntid, oniid, ojdtipo
            FROM gestaodocumentos.objetodemanda
            WHERE tarid = {$tarid}
        ";
        $objeto_demanda = $db->carregar($sql);

        if( $objeto_demanda != '' ){
            $sql = "
                DELETE FROM gestaodocumentos.objetodemanda WHERE tarid IN ({$reiter_impl});
            ";
            $db->executar($sql);

            foreach( $reiteracoes as $reit_tarid ){

                foreach( $objeto_demanda as $dados ){

                    $iesid = $dados['iesid'] != '' ? $dados['iesid'] : 'NULL';
                    $mntid = $dados['mntid'] != '' ? $dados['mntid'] : 'NULL';
                    $oniid = $dados['oniid'] != '' ? $dados['oniid'] : 'NULL';
                    $ojdtipo = $dados['ojdtipo'] != '' ? $dados['ojdtipo'] : 'NULL';

                    $sql = "
                        INSERT INTO gestaodocumentos.objetodemanda(
                                tarid, iesid, mntid, oniid, ojdtipo
                            )VALUES(
                                {$reit_tarid}, {$iesid}, {$mntid}, {$oniid}, '{$ojdtipo}'
                        ) RETURNING ojdid;
                    ";
                    $ojdid = $db->pegaUm($sql);

                }
            }
        }
    }

    if( $ojdid > 0 ){
        $db->commit();
        enviarEmailCoordenacao();
        return true;
    }else{
        return enviarEmailCoordenacao();
    }
}