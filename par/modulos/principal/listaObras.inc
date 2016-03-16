<?php

if($_REQUEST['requisicao'] == 'carreganomeescola'){

    $codigoEscola = $_REQUEST['codigoEscola'];
    if($codigoEscola)
    {
        $sql = "(select et.entcodent||' - '||et.entnome from entidade.entidade et where et.entcodent = '{$codigoEscola}' AND entstatus = 'A')";
        $escola = $db->pegaUm($sql);
    }
	?>
<table><tr><td><?php echo $escola?></td></tr></table>
<?php 
	exit;
}

if( $_SESSION['usucpf'] == '' && $_REQUEST['executa'] == 1 ){

	ini_set("memory_limit","2000M");
	set_time_limit(0);

	include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
	$oWSSigarp = new WSSigarp();

	$sql = "SELECT
				DISTINCT tpc.terid, tcp.proid, pre.estuf, iu.inuid
			FROM
				par.termoobraspaccomposicao tpc
			INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
			INNER JOIN par.termocompromissopac tcp ON tcp.terid = tpc.terid AND terstatus = 'A'
			INNER JOIN par.enviomiitemperdido eip ON eip.preid = tpc.preid
			INNER JOIN par.instrumentounidade iu ON iu.muncod = pre.muncod";
	$dados = $db->carregar($sql);
	if( is_array($dados) ){
		foreach( $dados as $dado ){
			$_SESSION['par']['inuid'] = $dado['inuid'];
			$oWSSigarp->solicitarManualmenteItemObra($dado['terid'], $dado['proid']);
			unset($_SESSION['par']['inuid']);
		}
	}

	die("FIM");
}

function unblockReformulacao(){
    global $db;
    $sql = "UPDATE obras.preobra SET preblockreformulacao = 'N' WHERE preid = {$_POST['preid']}";
    $db->executar($sql);
    echo ($db->commit()) ? "sim" : "nao";
    exit;
}

function listaReformulacoes($dados) {
	global $db;

	$sql = "SELECT 	
				'<img border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"abreReformulacao('||pre.preid||', '||pre.muncod||');\" style=\"cursor:pointer\" />' as acao,
				row_number() over() || 'ª' as id,
				pre.predescricao,
				pto.ptodescricao,
				usu.usunome,
				to_char(pre.predatareformulacao,'dd/mm/YYYY') as predatareformulacao
			FROM obras.preobra pre
			INNER JOIN obras.pretipoobra 	pto ON pto.ptoid = pre.ptoid
			INNER JOIN seguranca.usuario	usu ON usu.usucpf  = pre.preusucpfreformulacao
			WHERE 
				pre.preidpai = {$dados['preid']} 
				AND prestatus = 'A' 
				AND preid NOT IN (SELECT preid FROM obras.preobracancelamentoreformulacao)
			ORDER BY pre.predatareformulacao";
// ver($sql,d);
	$cabecalho = array("&nbsp;","Reprogramação", "Nome da obra", "Tipo de Obra", "Criador da reformulação", "Data dat Reformulação");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function reformularObra($dados) {
	
	global $db;

	include_once APPRAIZ . "includes/workflow.php";

	/* RESUMO para COPIAR O QUESTIONARIO
	buscar com preidpai o qrpid (preobraanalise)
	buscar questionario.questionarioresposta com o qrpid do pai.
	buscar questionario.resposta com o qrpidpai
	gerar novo preid -> referenciando o pai obras.preobra
	gerar novo qrpid trocando qrptitulo (questionario.questionarioresposta )
	gerar preobraanalise com qrpid e preid novos
	gerar novas resposta com o qrpid novo
	 */
	$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$dados['preid']."'");
	$result = wf_alterarEstado( $docid, $aedid = WF_AEDID_APROVADO_ENVIAR_EM_REFORMULACAO, "Reformulado em 'Lista de obras PAC2' ".date('d/m/Y')." por {$_SESSION['usucpf']}", $d = array( 'preid' => $dados['preid'] ));
	
	if($result) {
		
		echo "<script>
				alert('Reformulação criada com sucesso');
				window.location='par.php?modulo=principal/listaObras&acao=A';
		      </script>";

	} else {
		
		if( $result != '' ){
			$msg = $result;
		}else{
			$msg = "Tramitação não foi possivel. Possivelmente não possui perfil";
		}
		
		echo "<script>
				alert('$msg');
				window.location='par.php?modulo=principal/listaObras&acao=A';
		      </script>";
	}


}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}



ini_set( "memory_limit", "1024M" );
set_time_limit(0);

include APPRAIZ."includes/cabecalho.inc";
//include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

//$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAC );
monta_titulo( $titulo_modulo, $tabela_execucao );

$obPreObraControle = new PreObraControle();
if($_POST['pesquisa']=='1'){
	$post = $_POST;
}
$post['sisid'] = 23;
$post['tooid'] = 1;


    // Só monta lista quando clicar em pesquisar
if($_POST['exporta'] == "true" || isset($_REQUEST['estuf'])){
	$arObras = $obPreObraControle->recuperarListaObras($post);
}

$arObras = $arObras ? $arObras : array();

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_ListaObrasPAC".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$cabecalho = array("Preid", "Obrid", "Nome da obra","Tipo da obra","Município","UF","Situação","Situação - Obras 2.0","Usuário", "Data da Situação", "Analista", "Resolução/Carga", "Valor da Obra", "Processo");
	$db->monta_lista_tabulado($arObras,$cabecalho,100000000,5,'N','100%',$par2);

	/* ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("Preid", "Obrid", "Nome da obra","Tipo da obra","Município","UF","Situação","Situação - Obras 2.0","Usuário", "Data da Situação", "Analista", "Resolução/Carga", "Valor da Obra", "Processo");
	$formato = array("","","","","","","","","","","","","n");
	$arObras = is_array($arObras) ? $arObras : Array();
	$db->sql_to_excel($arObras, 'relListaObrasPAC', $cabecalho, $formato); */
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
<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="//cdn.datatables.net/1.10.2/js/jquery.dataTables.js" charset="utf8"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript" src="../par/js/dataTables.tableTools.js"></script>
<script type="text/javascript" src="../par/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

<script type="text/javascript">
    jQuery(document).ready(function(){
        //#FUNCAO dataTable() jQuery que monta a tabela dinamicamente através do html
        //#tabela, busca e paginacao do PAR
        jQuery('#listaParObras').dataTable({
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

        jQuery("#listaParObras_filter label :input").width(380); //definindo o tamanho do campo Buscar
        jQuery("#listaParObras_filter label :input").addClass('normal'); //add class formato padrão simec campo input
        jQuery("#listaParObras_filter label :input").on('keyup', function(){

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


	$(function() {
	    $('#formulario').submit(function() {
	    	document.getElementById('exporta').value = "false";
			selectAllOptions( document.getElementById( 'ptoid' ) );
	    });
	});

	function confirmarReformulacao(preid) {

		jQuery.ajax({
	   		type: 	"POST",
	   		url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
	   		data: 	'req_ajax_workflow=form_sugestaoTrocaPtoid&preid='+preid,
	   		async: 	false,
	   		success: function(msg){
				html = '<center>'+
							'<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >'+
							'<div style="margin-top:250px; align:center;">'+
								'<img border="0" title="Aguardando" src="../imagens/carregando.gif">Carregando...</div>'+
							'</div>'+
						'</center>'+
						'<form method="POST" id="formsugestaoTrocaPtoid" enctype="multipart/form-data" name="formsugestaoTrocaPtoid">'+
							msg+
						'</form>';
	   			jQuery( "#div_dialog" ).html(html);
	   			jQuery( '#div_dialog' ).show();
	   			jQuery( "#div_dialog" ).dialog({
					resizable: true,
					width: 700,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"Fechar": function() {
							jQuery( this ).dialog( "close" );
						},
						"Tramitar": function() {
							if( jQuery('#ptoids').find('[value!=""]').length < 1 ){
								alert('Escolha pelo menos um tipo de obra.');
								return false;
							};
							if( confirm('Deseja realmente reformular esta obra?') ){
								jQuery('#aguardando').show();
								jQuery('select[multiple="multiple"]').children().attr('selected', true);
								jQuery.ajax({
							   		type: 	"POST",
							   		url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
							   		data: 	'req_ajax_workflow=sugestaoTrocaPtoid&'+jQuery('#formsugestaoTrocaPtoid').serialize(),
							   		async: 	false,
// 							   		dataType: JSON,
							   		success: function(msg){
							   			data = jQuery.parseJSON(msg);
										if( data.boo == true ){
											if( data.msg != '' ){
										   		alert(data.msg);
											}
											window.location='par.php?modulo=principal/listaObras&acao=A&requisicao=reformularObra&preid='+preid;
										}else{
											if( data.msg != '' ){
										   		alert(data.msg);
											}else{
												alert('Erro ao realisar a tramitação.');
											}
										}
										jQuery( this ).dialog( "close" );
							   		}
								});
							}
						}
					}
				});
	   		}
		});
	}

	function abreReformulacao(preid, muncod) {
		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod,
					   	   'ProInfancia', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
	}

	function exibirReformulacao(preid, obj) {
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
				url: "par.php?modulo=principal/listaObras&acao=A",
				data: "requisicao=listaReformulacoes&preid="+preid,
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

	function exportarExcel(){
		document.getElementById('exporta').value = "true";
		$('#pesquisa').val('1');
		selectAllOptions( document.getElementById( 'ptoid' ) );
		document.getElementById('formulario').submit();
	}

	$(document).ready(function(){

		$('#pesquisar').click(function(){
			$('#pesquisa').val('1');
			$('#formulario').submit();
		});
                
                $('.btnblockreformulacao').click(function(){
                    var preid = $(this).attr('preid');
                    if (confirm("Voce tem certeza?")) {
                        $.ajax({
                            type: "POST",
                            url: "par.php?modulo=principal/listaObras&acao=A",
                            data: "requisicao=unblockReformulacao&preid="+preid,
                            async: false,
                            success: function(response){
                                if (response === 'sim') {
                                    alert('Obra desbloqueada com sucesso!');
                                } else {
                                    alert('Falha ao desbloquear obra!');
                                }
                                window.location = window.location;
                            }
                        });
                    }
                });
	});

	function mostrarDadosObras( preid, muncod, esdid ){

		if(esdid == 194 || esdid == 215){
			var aba = 'analiseEngenheiro';
		}else{
			var aba = 'dados';
		}

		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+'&preid='+preid+'&muncod='+muncod,
				   'ProInfancia',
				   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
	}
        
	function cancelarProrrogacaoPrazo(preid, popstatus){
            return window.open('par.php?modulo=principal/popupCancelarProrrogacaoPrazo&acao=A&preid='+preid+'&popstatus='+popstatus,
                               'Cancelar Prorrogação de Prazo',
                               "height=520,width=550,scrollbars=yes,top=50,left=200" ).focus();
	}
        
</script>

<div id="div_dialog" style="display:none" ></div>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
                <form method="post" name="formulario" id="formulario">
                    <input type="hidden" id="exporta" name="exporta" value="<?= $exporta; ?>">
                    <input type="hidden" id="pesquisa" name="pesquisa" value="">
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
                        <!--
                        <tr>
                            <td colspan="2">
                                Origem
                                <br/>
                                <?php
                                /* $sql = "SELECT
                                  tooid as codigo,
                                  toodescricao as descricao
                                  FROM
                                  obras.tipoorigemobra
                                  WHERE
                                  toostatus = 'A'
                                  ORDER BY
                                  2";

                                  $tooid = $_POST['tooid'];
                                  $db->monta_combo( "tooid", $sql, 'S', 'Selecione...', '', '' ); */
                                ?>
                            </td>
                        </tr>
                        -->
                        <tr>
                            <td colspan="2"> Esfera<br>
                                <?php
                                    $sql = "
                                        SELECT  DISTINCT ptoesfera as codigo,
                                                CASE
                                                    WHEN ptoesfera = 'E' THEN 'Estadual'
                                                    WHEN ptoesfera = 'M' THEN 'Municipal'
                                                END as descricao
                                        FROM obras.pretipoobra
                                        WHERE ptostatus = 'A' AND ptoesfera not in ('T')
                                        ORDER BY descricao
                                    ";
                                    $db->monta_checkbox('ptoesfera[]', $sql, $_POST['ptoesfera']);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Grupo de Município<br>
                                <?php
                                    $sql = "
                                        SELECT  tpmid as codigo,
                                                tpmdsc as descricao
                                        FROM territorios.tipomunicipio
                                        WHERE tpmstatus = 'A' AND gtmid = 7
                                        ORDER BY descricao
                                    ";
                                    $tpmid = $_POST['tpmid'];
                                    $db->monta_combo("tpmid", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Tipo de Obra<br>
                                <?php
                                    $sql = "
                                        SELECT  DISTINCT pto.ptoid as codigo,
                                                pto.ptodescricao as descricao
                                        FROM obras.pretipoobra pto
                                        INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
                                        WHERE pto.tooid = 1
                                        ORDER BY 2 desc
                                    ";
                                    if ($_POST['ptoid'][0]) {
                                        $where = " pto.ptoid IN (" . implode(", ", $_POST['ptoid']) . ") ";
                                        $sql2 = "
                                            SELECT  DISTINCT pto.ptoid as codigo,
                                                    pto.ptodescricao as descricao
                                            FROM obras.pretipoobra pto
                                            INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
                                            WHERE pto.tooid = 1 AND {$where}
                                            ORDER BY 2 desc
                                        ";
                                        $dados = $db->carregar($sql2);
                                    }else{
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
                                        WHERE tpdid = " . WF_FLUXO_PRO_INFANCIA . "
                                        ORDER BY esddsc 
                                    ";
                                    $esdid = $_POST['esdid'];
                                    $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Resolução
                                <br/>
                                <?php
                                    $resid = $_POST['resid'];
                                    $sql = "select resid as codigo, resdescricao as descricao from par.resolucao where resstatus='A'";
                                    $db->monta_combo("resid", $sql, 'S', 'Todas as Resoluções', '', '');
                                ?>
                            </td>
                        </tr>
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
                            <td colspan="2">
                                Ano de Seleção
                                <br/>
                                <?php
                                    $sql = "
                                        SELECT  DISTINCT preanoselecao as codigo,
                                                preanoselecao as descricao
                                        FROM obras.preobra
                                        WHERE prestatus = 'A' AND preanoselecao > 0
                                    ";
                                    $preanoselecao = $_POST['preanoselecao'];
                                    $db->monta_combo("preanoselecao", $sql, 'S', 'Selecione...', '', '');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                Arquivos Corrompidos:
                                <?php
                                    $sql = "
                                        SELECT  '0' as codigo,
                                                'Nenhum filtro' as descricao
                                        UNION ALL
                                        SELECT  '1' as codigo,
                                                'Possui arquivos corrompidos' as descricao
                                    ";
                                    //$db-> monta_checkbox('corrompido[]', $sql, $_POST['corrompido']);

                                    if(possuiPerfil(Array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_ADMINISTRADOR))){
                                        echo "<br>";
                                        $sql .= "
                                            UNION ALL
                                            SELECT  '2' as codigo,
                                                    'Possui arquivos corrompidos com substituição pendente' as descricao
                                            UNION ALL
                                            SELECT  '3' as codigo,
                                                    'Possui arquivos corrompidos, substituídos e com validação pendente' as descricao
                                            UNION ALL
                                            SELECT  '4' as codigo,
                                                    'Possui arquivos corrompidos, substituídos e validados' as descricao
                                            UNION ALL
                                            SELECT  '5' as codigo,
                                                    'Não possui arquivos corrompidos' as descricao
                                        ";
                                        //$db-> monta_checkbox('substituidos[]', $sql, $_POST['substituidos']);
                                    }
                                    $corrompido = $_REQUEST['corrompido'];
                                    $db->monta_radio('corrompido', $sql, 'S', 1)
                                ?>
                                <br>
                            </td>
                        </tr>
                        <!--
                        <tr>
                            <td colspan="2">
                                Empenho:
                                <br>
                                <label>
                                    <input type="checkbox" name="empenho" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['empenho'] == 1 ? 'checked="checked"' : '' ?>/> Com obra empenhada
                                </label>
                                <br>
                            </td>
                        </tr>
                        -->
                        <tr>
                            <td colspan="2">
                                <label>
                                    <input type="checkbox" name="premcmv" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['premcmv'] == 1 ? 'checked="checked"' : '' ?>/>
                                    Obras - Minha Casa Minha Vida
                                </label>
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
                            <td colspan="2">
                                Metodologia Inovadora:
                                <br />
                                <input <?php echo $_REQUEST['obrasmi'] == "1" ? "checked='checked'" : "" ?> type="radio" name="obrasmi" value="1" >Aderiu
                                <input <?php echo $_REQUEST['obrasmi'] == "2" ? "checked='checked'" : "" ?> type="radio" name="obrasmi" value="2" >Não aderiu
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Empenho:
                                <br />
                                <input <?php echo ($_REQUEST['empenhado'] == "1") ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" />Obra empenhada
                                <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" />Obra não empenhada
                                <input <?php echo ($_REQUEST['empenhado'] == "" || $_REQUEST['empenhado'] == "T") ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="T" />Todos
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Obras MUNICIPAIS com pendências:
                                <br />
                                <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                                <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                                <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                            </td>
                        </tr>
                        <tr>
                            <td valign="bottom">
                                Obras ESTADUAIS com pendências:
                                <br />
                                <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
                                <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
                                <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
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
                        <input type="button" id="pesquisar" value="Pesquisar" />
                        <input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
                    </div>
                </form>
            </td>
        </tr>
    </tbody>
</table>

<center>
<div class="conteudo_tabelas">
    <br><br><br>
    <table id="listaParObras" class="display tabela" cellspacing="0" width="95%">
        <thead>
            <tr>
                <th class="td_cabeca_lista" width="7%">Ação</th>
                <th class="td_cabeca_lista" width="5%">Preid</th>
                <th class="td_cabeca_lista" width="5%">Obrid</th>
                <th class="td_cabeca_lista" width="10%">Nome da obra</th>
                <th class="td_cabeca_lista" width="10%">Tipo de obra</th>
                <th class="td_cabeca_lista" width="8%">Município</th>
                <th class="td_cabeca_lista" width="4%">UF</th>
                <th class="td_cabeca_lista" width="6%">Situação</th>
                <th class="td_cabeca_lista" width="6%">Situação - Obras 2.0</th>
                <th class="td_cabeca_lista">Escola</th>
                <th class="td_cabeca_lista" width="9%">Usuário</th>
                <th class="td_cabeca_lista" width="6%">Data da Situação</th>
                <th class="td_cabeca_lista" width="9%">Analista</th>
                <th class="td_cabeca_lista" width="6%">Resolução/ Carga</th>
                <th class="td_cabeca_lista" width="8%">Valor da Obra</th>
                <th class="td_cabeca_lista" width="8%">Processo</th>
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
                    <td><?= $resultadoObras['preid']; ?></td>
                    <td><?= $resultadoObras['obrid']; ?></td>
                    <td><?= $resultadoObras['predescricao']; ?></td>
                    <td><?= $resultadoObras['ptodescricao']; ?></td>
                    <td><?= $resultadoObras['mundescricao']; ?></td>
                    <td style="text-align:center;"><?= $resultadoObras['estuf']; ?></td>
                    <td><?= $resultadoObras['estado1']; ?></td>
                    <td><?= $resultadoObras['estado2']; ?></td>
                    <td><?= $resultadoObras['nome_escola']; ?></td>
                    <td><?= $resultadoObras['usunome']; ?></td>
                    <td><?= $resultadoObras['htddata']; ?></td>
                    <td><?= $resultadoObras['nomeanalista']; ?></td>
                    <td><?= $resultadoObras['resnumero']; ?></td>
                    <td style="text-align:right;"><?= formata_valor($resultadoObras['prevalorobra'], 2); ?></td>
                    <td><?= $resultadoObras['processo']; ?></td>
                </tr>
            <?  } ?>
        </tbody>
    </table>
</div>
</center>