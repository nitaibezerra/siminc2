<?php

ini_set( "memory_limit", "1024M" );
set_time_limit(0);

function carreganomeescola ($arrData){
    global $db;
    $codigoEscola = $arrData['codigoEscola'];
    if($codigoEscola)
    {
        $sql = "(select et.entcodent||' - '||et.entnome from entidade.entidade et where et.entcodent = '{$codigoEscola}' AND entstatus = 'A')";
        $escola = $db->pegaUm($sql);
    }
    ?>
        <table>
            <tr>
                <td>
                    <?php echo $escola?>
                </td>
            </tr>
        </table>
    <?php 
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


function listaReformulacoes($dados) {
	global $db;
        $acoes_possui_reformulacao = "|| CASE WHEN (SELECT COUNT(po.preid) FROM obras.preobra po WHERE po.preidpai = pre.preid AND prestatus = 'A' AND preid NOT IN (SELECT preid FROM obras.preobracancelamentoreformulacao)) > 0 THEN ' <img src=../imagens/mais.gif title=abrir style=cursor:pointer; onclick=\"exibirReformulacao('||pre.preid||', this);\">' ELSE '' END";
        
	$sql = "SELECT 	'<img border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"exibir('||pre.preid||','||'".$dados['sbaid']."'||','||'".$dados['ano']."'||');\" style=\"cursor:pointer\" />' $acoes_possui_reformulacao as acao,
					row_number() over() || 'ª' as id,
					pre.predescricao,
					usu.usunome,
					to_char(pre.predatareformulacao,'dd/mm/YYYY') as predatareformulacao
				FROM obras.preobra pre
				INNER JOIN seguranca.usuario usu ON usu.usucpf  = pre.preusucpfreformulacao
				WHERE pre.preidpai = '".$dados['preid']."' 
                                    AND prestatus = 'A' 
                                    AND pre.preid NOT IN (SELECT preid FROM obras.preobracancelamentoreformulacao)
                                    AND pre.tooid <> 1
				ORDER BY pre.predatareformulacao";

	$cabecalho = array("&nbsp;","Reprogramação","Nome da obra", "Criador da reformulação", "Data dat Reformulação");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
        exit;
}


include APPRAIZ."includes/cabecalho.inc";

include_once APPRAIZ."par/classes/controle/PreObraControle.class.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAR );
monta_titulo( $titulo_modulo, $tabela_execucao );

$obPreObraControle = new PreObraControle();

if($_POST['pesquisa'])
{
	$post = $_POST;
}


// Só monta lista quando clicar em pesquisar
if($_POST['exporta'] == "true" || isset($_REQUEST['estuf'])){
///$inner = "INNER JOIN par.subacaoobra sbo ON sbo.preid = pre.preid";
    $arObras = $obPreObraControle->recuperarListaObrasPar($post, $inner);
} else {
    $arObras = array();
}

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_ListaObrasPAR".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$cabecalho = array("Preid", "Obrid", "Nome da Obra", "Tipo da Obra","Município", "UF","Descrição da Subação", "Situação", "Situação - Obras 2.0", "Usuário", "Ano da Obra", "Analista", "Valor da Obra", "Processo");
	$db->monta_lista_tabulado($arObras,$cabecalho,100000000,5,'N','100%',$par2);

	/* header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("Preid", "Obrid", "Nome da Obra", "Tipo da Obra","Município", "UF","Descrição da Subação", "Situação", "Situação - Obras 2.0", "Usuário", "Ano da Obra", "Analista", "Valor da Obra", "Processo");
	$db->sql_to_excel($arObras, 'relListaObrasPAR', $cabecalho, $formato); */
	exit;

}
$exporta = "false";

?>

<link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>

<link rel="stylesheet" href="../par/css/jquery.dataTables.css" type="text/css" media="all" />
<link rel="stylesheet" href="../par/css/dataTables.tableTools.css" type="text/css" media="all" />
<link rel="stylesheet" href="../par/css/jquery.dataTables.min.css" type="text/css" media="all" />
<link rel="stylesheet" href="../par/css/jquery.dataTables_themeroller.css" type="text/css" media="all" />
<link rel="stylesheet" href="../par/css/jquery.dataTables.simec-custom.css" type="text/css" media="all" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>


<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js">-->
<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<!--<script type="text/javascript" src="../includes/funcoes.js" ></script>-->
<script type = "text/javascript" charset = "utf8" src = "//cdn.datatables.net/1.10.2/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript" language="javascript" src="../par/js/dataTables.tableTools.js"></script>
<script type="text/javascript" language="javascript" src="../par/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->

<script type="text/javascript">
    jQuery(document).ready(function(){
    	
    	
        //#FUNCAO dataTable() jQuery que monta a tabela dinamicamente através do html
        //#tabela, busca e paginacao do PAR
        jQuery('#listaPar').dataTable({
            "pagingType": "full_numbers",//mostra a paginação através de números em vez das setas
            "language": {
                "sProcessing"   : "Processando...",
                "sLengthMenu"   : "Mostrar _MENU_ registros",
                "sZeroRecords"  : "N&atilde;o foram encontrados resultados",
                "sInfo"         : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
                "sInfoEmpty"    : "Mostrando de 0 at&eacute; 0 de 0 registros",
                "sInfoFiltered" : "(filtrado de _MAX_ registros no total)",
                "sInfoPostFix"  : "",
                "sSearch"       : "Buscar:",
                "sUrl"          : "",
                "oPaginate": {
                    "sFirst"    : "Primeiro",
                    "sPrevious" : "Anterior",
                    "sNext"     : "Seguinte",
                    "sLast"     : "&Uacute;ltimo"
                }
            },//altera o idioma para português
            "bJQueryUI" : false,//habilita o uso do Datatables com o tema do JQueryUI
            "bFilter"   : true, //habilitando campo de busca
            "sDom"      : 'T<"clear">frtip',
            responsive  : true,
            tableTools: {
                "aButtons": []
            }
        });

        jQuery("#listaPar_filter label :input").width(380); //definindo o tamanho do campo Buscar
        jQuery("#listaPar_filter label :input").addClass('normal'); //add class formato padrão simec campo input
        jQuery("#listaPar_filter label :input").on('keyup', function(){

        	var searchTerm = jQuery(this).val().trim().split(" ");

    		jQuery('#listaPar tbody tr').each(function(){
    			jQuery(this).children().each(function(){
    				
    				var html = jQuery(this).find('span').html();

    				if( html ){
    					html = replaceAll(html, '<b>','');
    					html = replaceAll(html, '</b>','');
    		
    					if( searchTerm != '' ){
    						for(var i = 0; i < searchTerm.length; i++) {
    							var pattern = "([^\\w]*)(" + searchTerm[i] + ")([^\\w]*)";
    							var rg = new RegExp(pattern);
    							var match = rg.exec(html);
    							
    							if(match) {
    								html = html.replace(rg,match[1] + "<b>"+ match[2] +"</b>" + match[3]);
    							}
    						}
    					}
    					jQuery(this).children('span').html(html);
    				}
    			});
    		});
        });
    });

    $(function(){
        $('#formulario').submit(function(){
            document.getElementById('exporta').value = "false";
            selectAllOptions( document.getElementById( 'ptoid' ) );
            selectAllOptions( document.getElementById( 'tpmid' ) );
        });
    });

    function exibir(preid, sbaid, sobano){
        var url = 'par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&sbaid='+sbaid+'&ano='+sobano;
        return window.open(url, 'subacaoobras', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
    }

    function exportarExcel(){
        document.getElementById('exporta').value = "true";
        selectAllOptions( document.getElementById( 'ptoid' ) );
        selectAllOptions( document.getElementById( 'tpmid' ) );
        document.getElementById('formulario').submit();
    }
    
    function exibirReformulacao(preid, sbaid, ano, obj) {
        var linha  = obj.parentNode.parentNode;
        var tabela = obj.parentNode.parentNode.parentNode;
        if(obj.title == 'abrir') {

            obj.title = 'fechar';
            obj.src = '../imagens/menos.gif';

            var linhan = tabela.insertRow(linha.rowIndex);
            var col0 = linhan.insertCell(0);
            col0.innerHTML = '&nbsp;';
            var col1 = linhan.insertCell(1);
            col1.colSpan=9;

            $.ajax({
                type: "POST",
                url: "par.php?modulo=principal/listaObrasPar&acao=A",
                data: "requisicao=listaReformulacoes&preid="+preid+"&sbaid="+sbaid+"&ano="+ano,
                async: false,
                success: function(msg){
                    col1.innerHTML = msg;
                }
            });

        } else {
            obj.title = 'abrir';
            obj.src = '../imagens/mais.gif';

            tabela.deleteRow(linha.rowIndex);

        }
    }
    
    function cancelarProrrogacaoPrazo(preid){
            return window.open('par.php?modulo=principal/popupCancelarProrrogacaoPrazo&acao=A&preid='+preid,
                               'Cancelar Prorrogação de Prazo',
                               "height=520,width=550,scrollbars=yes,top=50,left=200" ).focus();
	}
</script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
                <form method="post" name="formulario" id="formulario">
                    <input type="hidden" name="tooid" id="tooid" value="2" />
                    <input type="hidden" id="exporta" name="exporta" value="<?= $exporta; ?>">
                    <table border="0" cellpadding="3" cellspacing="0">
                        <tr>
                            <td colspan="2">
                                Preid
                                <br/>
                                <?php echo campo_texto('preid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['preid']); ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Obrid
                                <br/>
                                <?php echo campo_texto('obrid', 'N', 'S', 'Obrid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Nome da obra
                                <br/>
                                <?php
                                    $filtro = htmlentities($_POST['predescricao']);
                                    $predescricao = $filtro;
                                    echo campo_texto('predescricao', 'N', 'S', '', 80, 200, '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Município
                                <br/>
                                <?php
                                    $filtro = htmlentities($_POST['municipio']);
                                    $municipio = $filtro;
                                    echo campo_texto('municipio', 'N', 'S', '', 50, 200, '', '');
                                ?>
                            </td>
                            <td valign="bottom">
                                Estado
                                <br/>
                                <?php
                                    $estuf = $_POST['estuf'];
                                    $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                                    $db->monta_combo("estuf", $sql, 'S', 'Todas as Unidades Federais', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Ano da Obra
                                <br/>
                                <?php
                                    $anos = array(
                                        array('codigo' => 2011, 'descricao' => 2011),
                                        array('codigo' => 2012, 'descricao' => 2012),
                                        array('codigo' => 2013, 'descricao' => 2013),
                                        array('codigo' => 2014, 'descricao' => 2014)
                                    );

                                    $anoatual = $_POST['anoatual'];
                                    $db->monta_combo("anoatual", $anos, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2"> Esfera<br>
                                <?php
                                    $ckE = "";
                                    $ckM = "";
                                    $ckT = "";
                                    if ($_POST['ptoesfera'] == 'E') {
                                        $ckE = 'checked';
                                    } elseif ($_POST['ptoesfera'] == 'M') {
                                        $ckM = 'checked';
                                    } elseif ($_POST['ptoesfera'] == 'T') {
                                        $ckT = 'checked';
                                    }
                                ?>
                                <input type='radio' value='E' name='ptoesfera' <?= $ckE; ?> />Estadual <input type='radio' value='M' name='ptoesfera' <?= $ckM; ?>/>Municipal <input type='radio' value='T' name='ptoesfera' <?= $ckT; ?>/>Todos
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Grupo de Município<br>
                                <?php
                                    $stSql = "
                                        SELECT  tpmid AS codigo,
						tpmdsc AS descricao
                                        FROM territorios.tipomunicipio
                                        WHERE tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)
                                    ";

                                    if($_REQUEST['tpmid'][0] != ''){
                                        $grupos = implode($_REQUEST['tpmid'], ",");
                                        $sql = "
                                            SELECT  tpmid AS codigo,
                                                    tpmdsc AS descricao
                                            FROM territorios.tipomunicipio mtq
                                            WHERE tpmid IN ({$grupos})
                                        ";
                                        $tpmid = $db->carregar($sql);
                                    }
                                    combo_popup("tpmid", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Tipo de Obra<br>
                                <?php
                                    $sql = "SELECT ptoid as codigo, ptodescricao as descricao FROM obras.pretipoobra WHERE tooid <> 1 and ptostatus = 'A' ORDER BY 2 desc";
                                    if ($_POST['ptoid'][0]) {
                                        $where = " pto.ptoid IN (" . implode(", ", $_POST['ptoid']) . ") ";
                                        $sql2 = "SELECT DISTINCT pto.ptoid as codigo, pto.ptodescricao as descricao FROM obras.pretipoobra pto WHERE pto.tooid = 2 AND {$where} ORDER BY 2 desc";
                                        $dados = $db->carregar($sql2);
                                    } else {
                                        $dados = "";
                                    }
                                    combo_popup('ptoid', $sql, 'Selecione...', "400x400", 0, array(), "", (($entcodent) ? "N" : "S"), false, false, 5, 400, null, null, '', '', $dados, true, false, '', '', Array('descricao'), Array('dsc'));
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Situação
                                <br/>
                                <?php
                                    $sql = "
                                        SELECT  esdid as codigo,
                                                esddsc as descricao
                                        FROM workflow.estadodocumento
                                        WHERE tpdid = " . WF_FLUXO_OBRAS_PAR . "
                                        ORDER BY esddsc
                                    ";
                                    $esdid = $_POST['esdid'];
                                    $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <!--
                        <tr>
                            <td colspan="2">
                                Equipe
                                <br/>
                                <?php
                                    $sql = "
                                        SELECT  DISTINCT pu.usucpf as codigo,
                                                u.usunome as descricao
                                        FROM seguranca.perfilusuario pu
                                        INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
                                        WHERE pu.pflcod IN (" . PAR_PERFIL_ENGENHEIRO_FNDE . ", " . PAR_PERFIL_COORDENADOR_GERAL . ", " . PAR_PERFIL_COORDENADOR_TECNICO . ")
                                        ORDER BY u.usunome
                                    ";
                                    $usucpf = $_POST['usucpf'];
                                    $db->monta_combo("usucpf", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        -->
                        <tr>
                            <td colspan="2">
                                Técnicos - análises efetuadas
                                <br/>
                                <?php
                                    $sql = "
                                        SELECT  DISTINCT poa.poausucpfinclusao as codigo,
                                                u.usunome as descricao
                                        FROM seguranca.perfilusuario pu
                                        INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
                                        INNER JOIN obras.preobraanalise poa ON poa.poausucpfinclusao = u.usucpf
                                        WHERE pu.pflcod IN (" . PAR_PERFIL_ENGENHEIRO_FNDE . ")
                                        ORDER BY u.usunome
                                    ";
                                    $poausucpfinclusao = $_POST['poausucpfinclusao'];
                                    $db->monta_combo("poausucpfinclusao", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Empenho:
                                <br>
                                <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >Obra empenhada
                                <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >Obra não empenhada
                                <input <?php echo ($_REQUEST['empenhado'] == "" || $_REQUEST['empenhado'] == "T") ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="T" >Todos
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Pendências no Monitoramento de Obras:
                                <br />
                                <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                                <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                                <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <label>
                                    <input type="checkbox" name="reformulacao" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['reformulacao'] == 1 ? 'checked="checked"' : '' ?>/>
                                    Obras Reformuladas
                                </label>
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                <label>Termo: </label><br />
                                    <input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : ""?> type="radio" name="termogerado" id="termogerado_1" value="1">Gerado
                                    <input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : ""?> type="radio" name="termogerado" id="termogerado_2" value="2">Não Gerado 
                                    <input <?php echo empty($_REQUEST['termogerado']) ? "checked='checked'" : ""?> type="radio" name="termogerado" value="">Todos
                            </td>
			</tr>
                        <tr>
                            <td valign="bottom">
                                <label>Pagamento: </label><br />
                                    <input <?php echo $_REQUEST['pagamento'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pagamento" id="pagamento_1" value="1" />Pagamento Solicitado
                                    <input <?php echo $_REQUEST['pagamento'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pagamento" id="pagamento_2" value="2" />Pagamento Não Solicitado
                                    <input <?php echo $_REQUEST['pagamento'] == "3" ? "checked='checked'" : "" ?> type="radio" name="pagamento" id="pagamento_3" value="3" />Pagamento efetivado
                                    <input <?php echo empty($_REQUEST['pagamento']) ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="" />Todos
                                
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                <label>Obras Migradas para sistema do Obras 2:</label><br />
                                    <input <?php echo $_REQUEST['obrasmigradas'] == "1" ? "checked='checked'" : "" ?> type="radio" name="obrasmigradas" value="1" />Sim
                                    <input <?php echo $_REQUEST['obrasmigradas'] == "2" ? "checked='checked'" : "" ?> type="radio" name="obrasmigradas" value="2" />Não
                                    <input <?php echo empty($_REQUEST['obrasmigradas']) ? "checked='checked'" : "" ?> type="radio" name="obrasmigradas" value="" />Todos
                            </td>
                        </tr>
                    </table>
                    <div style="float: left;">
                        <input type="submit" name="pesquisar" value="Pesquisar" />
                        <input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
                        <input type="hidden" name="pesquisa" value="1>"/>
                    </div>
                </form>
            </td>
        </tr>
    </tbody>
</table>
<center>
<div class="conteudo_tabelas">
    <br><br><br>
    <table id="listaPar" class="display tabela" cellspacing="0" width="95%">
        <thead>
            <tr>
                <th class="td_cabeca_lista" width="4%">Ação</th>
                <th class="td_cabeca_lista" width="4%">Preid</th>
                <th class="td_cabeca_lista" width="4%">Obrid</th>
                <th class="td_cabeca_lista" width="8%">Nome da obra</th>
                <th class="td_cabeca_lista" width="8%">Tipo de obra</th>
                <th class="td_cabeca_lista" >Município</th>
                <th class="td_cabeca_lista" width="3%">UF</th>
                <th class="td_cabeca_lista" >Descrição da Subação</th>
                <th class="td_cabeca_lista" width="9%">Situação</th>
                <th class="td_cabeca_lista" >Situação - Obras 2.0</th>
                <th class="td_cabeca_lista" >Escola</th>
                <th class="td_cabeca_lista" >Usuário</th>
                <th class="td_cabeca_lista" >Ano da Obra</th>
                <th class="td_cabeca_lista" >Analista</th>
                <th class="td_cabeca_lista" >Valor da Obra</th>
                <th class="td_cabeca_lista" >Processo</th>
            </tr>
        </thead>
        <tbody>
            <?php
                $c = 0;
                foreach($arObras as $resultadoObras ){
                    $c++;
            ?>
            <tr class="tr_corpo_lista">
                    <td><?= $resultadoObras['acao']; ?></td>
                    <td><span><?= $resultadoObras['preid']; ?></span></td>
                    <td><span><?= $resultadoObras['obrid']; ?></span></td>
                    <td><span><?= $resultadoObras['predescricao']; ?></span></td>
                    <td><span><?= $resultadoObras['ptodescricao']; ?></span></td>
                    <td><span><?= $resultadoObras['mundescricao']; ?></span></td>
                    <td style="text-align:center;"><span><?= $resultadoObras['estuf']; ?></span></td>
                    <td><span><?= $resultadoObras['sbadsc']; ?></span></td>
                    <td><span><?= $resultadoObras['estado1']; ?></span></td>
                    <td><span><?= $resultadoObras['estado2']; ?></span></td>
                    <td><span><?= $resultadoObras['nome_escola']; ?></span></td>
                    <td><span><?= $resultadoObras['usunome']; ?></span></td>
                    <td><span><?= $resultadoObras['sobano']; ?></span></td>
                    <td><span><?= $resultadoObras['nomeanalista']; ?></span></td>
                    <td style="text-align:right;"><span><?= $resultadoObras['prevalorobra']; ?></span></td>
                    <td><span><?= $resultadoObras['processo']; ?></span></td>
                </tr>
            <?  } ?>
        </tbody>
    </table>
</div>
</center>