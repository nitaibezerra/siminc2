<?php
include '_funcoes_termo.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

function formRegeraTermo($req){
	global $db;

	extract($_REQUEST);
	?>
	<center><b>Deseja regerar este termo?</b></center>
	<form method="post" name="formRegerar" id="formRegerar">
		<?=listaPreObrasTermo($_REQUEST) ?>
	</form>
<?php 
}

function formTermoAditivo($req){

	global $db;

	extract($_REQUEST);
	?>
	<center><b>Deseja gerar um termo de compromisso de reformulação?</b></center>
	<form method="post" name="formTermoAditivo" id="formTermoAditivo">
		<?=listaPreObrasTermo($_REQUEST) ?>
	</form>
<?php 
}
$exportaXLS = false;

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
//
if($_REQUEST['tipopesquisa'])
{
	if($_REQUEST['tipopesquisa'] == 'xls')
	{
		$exportaXLS = true;
	}
}

include APPRAIZ."includes/cabecalho.inc";

echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );

$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAC );
monta_titulo( $titulo_modulo, $tabela_execucao );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script type="text/javascript" src="./js/par.js"></script>

<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<script>
function excluirAnexoTermo(codigo, tipo) {
	var conf = confirm('Deseja excluir o anexo?');
	if(conf) {
		window.location='par.php?modulo=principal/termoPac&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/termoPac&acao=A'); ?>&requisicao=excluiranexotermo&codigo='+codigo+'&tipo='+tipo;
	}
}

function enviarAnexo(terid) {
	displayMessage('par.php?modulo=principal/termoPac&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/termoPac&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermo&terid='+terid);
}

function enviarAnexo2(proid) {
	displayMessage('par.php?modulo=principal/termoPac&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/termoPac&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermoprocesso&proid='+proid);
}

function assinarTermo(obj) {
	var assinado="";
	if(obj.checked == true) {
		assinado="&terassinado=TRUE";
	} else {
		assinado="&terassinado=FALSE";
	}

	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/termoPac&acao=A",
		data: "requisicao=assinarTermo&terid="+obj.value+assinado,
		async: false,
		success: function(msg){}
	});
}


function enviaFormulario(tiporequisicao){
    selectAllOptions(document.getElementById('tipoobra'));
	$('#tipopesquisa').val(tiporequisicao);
	
	$('#req').val('pesquisar');
	$('#formulario').submit();
}

$(document).ready(function(){

	$('.historicoTermo').live('click', function(){

		var src = $(this).attr('src');
		var terid = $(this).attr('id');
		var linha = $(this).parent().parent();
		
		if( src == '../imagens/mais.gif' ){
			$(this).attr('src', '../imagens/menos.gif');

			$.ajax({
				type: "POST",
				url: window.location,
				data: "requisicao=listaTermoFilho&terid="+terid+"&titid=1",
				async: false,
				success: function(msg){
					$(linha).after('<tr id=tr_'+terid+'><td colspan="7">'+msg+'</td></tr>');
				}
			});

		}else{
			$(this).attr('src', '../imagens/mais.gif');

			$('#tr_'+terid).remove();
		}
	});
	
	$('.aditivoTermo').live('click', function(){
		var variaveis 	= $(this).attr('id').split('_');
		var proid 		= variaveis[0];
		var protipo 	= variaveis[1];
		var muncod 		= variaveis[2]; 
		var estuf 		= variaveis[3];  
		var terid 		= variaveis[4];
                var acaoorigem = $(this).attr('attr');

		if( muncod == 'null'||muncod == '' ){
			muncod = '';
		}else{
			muncod = '&muncod='+ muncod;
		}
		
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/termoPac&acao=A",
			data: "requisicao=formTermoAditivo&tipo=simples&proid="+proid+"&muncod="+muncod+"&estuf="+estuf+"&terid="+terid,
			async: false,
			success: function(msg){
                                if (acaoorigem == 'REPROGRAMAR') {
                                    $("#dialogAditivo").attr('title', 'Reprogramar Termo de Compromisso');
                                } else {
                                    $("#dialogAditivo").attr('title', 'Regerar Termo de Compromisso');
                                }
				$( "#dialogAditivo" ).html(msg);	
				
				$( "#dialogAditivo" ).dialog({
					resizable: true,
					width:600,
					modal: true,
					buttons: {
						"Sim": function() {

							var arPreid = '';
							$('#formTermoAditivo').find('[name="preids[]"]').each(function(i, obj){
								if(arPreid==''){
									arPreid = $(obj).val();
								}else{
									arPreid += ","+$(obj).val();
								}
							});
							if(arPreid=='') {
								alert('Nenhuma obra carregada');
								$( "#dialogAditivo" ).html('')
								return false;
							}
							window.open('par.php?modulo=principal/modeloTermoAditivoObra&acao=A&termoaditivo=true&proid='+proid+'&arPreid='+arPreid+muncod+'&estuf='+estuf+'&tipoobra='+protipo+"&acaoorigem="+acaoorigem, 
							        	'modelo', 
							       	 	"height=600,width=400,scrollbars=yes,top=0,left=0" );
				       	 	window.location.href = window.location;
							$( this ).dialog( "close" );
						},
						"Não": function() {
							$( this ).dialog( "close" );
						}
		
					}
				});
			}
		});
		
	});
	
	
	$('.regerarTermo').live('click', function(){

		var variaveis 	= $(this).attr('id').split('_');
		var proid 		= variaveis[0];
		var protipo 	= variaveis[1];
		var muncod 		= variaveis[2]; 
		var estuf 		= variaveis[3];  
		var terid 		= variaveis[4];

		if( muncod == 'null'||muncod == '' ){
			muncod = '';
		}else{
			muncod = '&muncod='+ muncod;
		}
		
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/termoPac&acao=A",
			data: "requisicao=formRegeraTermo&tipo=simples&proid="+proid+"&muncod="+muncod+"&estuf="+estuf+"&terid="+terid,
			async: false,
			success: function(msg){
				$( "#dialog" ).html(msg);	
				
				$( "#dialog" ).dialog({
					resizable: true,
					width:400,
					modal: true,
					buttons: {
						"Sim": function() {

							var arPreid = '';
							$('#formRegerar').find('[name="preids[]"]').each(function(i, obj){
								if(arPreid==''){
									arPreid = $(obj).val();
								}else{
									arPreid += ","+$(obj).val();
								}
							});
							if(arPreid=='') {
								alert('Nenhuma obra carregada');
								$( "#dialog" ).html('')
								return false;
							}
							
							window.open('par.php?modulo=principal/modeloTermoObra&acao=A&regerar=true&proid='+proid+'&arPreid='+arPreid+muncod+'&estuf='+estuf+'&tipoobra='+protipo, 
							        	'modelo', 
							       	 	"height=600,width=400,scrollbars=yes,top=0,left=0" );
				       	 	
							$( this ).dialog( "close" );
						},
						"Não": function() {
							$( this ).dialog( "close" );
						}
		
					}
				});
			}
		});
		
	});
});

</script>
<div id="dialog" title="Recarregar Termo" style="display:none;"></div>
<div id="dialogAditivo" title="Reprogramar Termo de Compromisso de Reformulação" style="display:none;"></div>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="req" id="req" value=""/>
	<input type="hidden" name="tipopesquisa" id="tipopesquisa" value=""/>
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
                        <tr>
                            <td class="SubTituloDireita">Preid:</td>
                            <td><?php echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['preid']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Obrid:</td>
                            <td><?php echo campo_texto( 'obrid', 'N', 'S', 'Obrid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?></td>
                        </tr>
            <tr>
				<td class="SubTituloDireita">Número do Termo:</td>
				<td colspan="3">
				<?php
					$ternumero = $_REQUEST['ternumero'];
					echo campo_texto( 'ternumero', 'N', 'S', '', 20, 50, '[#]', '');
				?>
				</td>
		
			</tr>
			<tr>
				<td class="SubTituloDireita">Número de Processo:</td>
				<td colspan="3">
				<?php
					$filtro = simec_htmlentities( $_REQUEST['empnumeroprocesso'] );
					$empnumeroprocesso = $filtro;
					echo campo_texto( 'empnumeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
				?>
				</td>
				
			</tr>
			<tr>
				<td class="SubTituloDireita">Município:</td>
				<td>
					<?php 
						$filtro = simec_htmlentities( $_POST['municipio'] );
						$municipio = $filtro;
						echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estado:</td>
				<td>
					<?php
						$estuf = $_POST['estuf'];
						$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
						$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
					?>
				</td>
			</tr>
						<!--tr>
							<td valign="bottom" colspan="2">
								Resolução
								<br/>
								<?php 
//									$resid = $_POST['resid'];
//									$sql = "select resid as codigo, resdescricao as descricao from par.resolucao where resstatus='A'";
//									$db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
								?>
							</td>
						</tr-->
			<tr>
				<td class="SubTituloDireita">Tipo de Obra:</td>
				<td>
                                    <?php
                                    $arrTipoObra = array();
                                    if ($_REQUEST['tipoobra'][0]) {
                                        $sql = "SELECT DISTINCT
                                                    pto.ptoid as codigo,
                                                    pto.ptodescricao as descricao
                                                FROM
                                                    obras.pretipoobra pto
                                                WHERE pto.tooid = 1
                                                    AND ptostatus = 'A'
                                                    AND ptoid IN (".implode(',', $_REQUEST['tipoobra']).")
                                                ORDER BY
                                                    pto.ptodescricao";
                                        $arrTipoObra = $db->carregar($sql);
                                    }
                                    
                                    $sql_combo = "SELECT DISTINCT
                                                    pto.ptoid as codigo,
                                                    pto.ptodescricao as descricao
                                                FROM
                                                    obras.pretipoobra pto
                                                WHERE pto.tooid = 1
                                                    AND ptostatus = 'A'
                                                ORDER BY
                                                    pto.ptodescricao";

                                    combo_popup( 'tipoobra', $sql_combo, 'Selecione', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrTipoObra, true, false, '', true);
                                    ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Termo:</td>
				<td>
					<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="1" />Termo Gerado
					<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="2" />Termo Não Gerado
					<?if(empty($_REQUEST['termogerado'])){ ?>
						<input <?php echo $_REQUEST['termogerado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="3" checked="checked" />Todos
					<?}else{ ?>
						<input <?php echo $_REQUEST['termogerado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="3" />Todos
					<?} ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assinatura:</td>
				<td>
					<input <?php echo $_REQUEST['assinado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="1" />Assinado
					<input <?php echo $_REQUEST['assinado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" />Não Assinado
					<?if(empty($_REQUEST['assinado'])){ ?>
						<input <?php echo $_REQUEST['assinado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="3" checked="checked" />Todos
					<?}else{ ?>
						<input <?php echo $_REQUEST['assinado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="3" />Todos
					<?} ?>
					
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Esfera:</td>
				<td>
					<?php 
						$esfera = $_POST['esfera'];
						$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
						$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Usuário de Criação:</td>
				<td>
					<?php 
						$usuario = $_POST['usuario'];
						$sql = "SELECT DISTINCT u.usucpf as codigo, u.usunome as descricao FROM seguranca.usuario u INNER JOIN par.termocompromissopac ter ON ter.usucpf = u.usucpf AND ter.terstatus='A' and ter.tpdcod = 102 ORDER BY u.usunome";
						$db->monta_combo( "usuario", $sql, 'S', 'Todas os usuários', '', '' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Anexo de publicação:</td>
				<td>
					<input <?php echo $_REQUEST['anexopublicacao'] == "1" ? "checked='checked'" : "" ?> type="radio" name="anexopublicacao" value="1" />Sim
					<input <?php echo $_REQUEST['anexopublicacao'] == "2" ? "checked='checked'" : "" ?> type="radio" name="anexopublicacao" value="2" />Não
					<?if(empty($_REQUEST['assinado'])){ ?>
						<input <?php echo $_REQUEST['anexopublicacao'] == "3" ? "checked='checked'" : "" ?> type="radio" name="anexopublicacao" value="3" checked="checked" />Todos
					<?}else{ ?>
						<input <?php echo $_REQUEST['anexopublicacao'] == "3" ? "checked='checked'" : "" ?> type="radio" name="anexopublicacao" value="3" />Todos
					<?} ?>
				</td>
			</tr>
            <tr>
                <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
                <td>
                    <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                    <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                    <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
                <td>
                    <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
                    <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
                    <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
                </td>
            </tr>
			<tr>
				<td class="subtitulodireita"></td>
				<td colspan="3">		
					<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar" onclick="enviaFormulario('pesquisa');" />
					<input type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/termoPac&acao=A';" />
					<input type="button" name="todos" value="Exportar Excel" onclick="enviaFormulario('xls');"  />
				</td>
			</tr>
		</tbody>
	</table>
</form>
<?php

if( $_REQUEST['req'] == 'pesquisar' ) {
	
	$wheremun[] = "m.muncod IS NOT NULL";
	$whereest[] = "p.estuf IS NOT NULL AND p.muncod is null";
	
	if($_REQUEST['empnumeroprocesso']){
		$_REQUEST['empnumeroprocesso'] = str_replace(".","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("/","", $_REQUEST['empnumeroprocesso']);
		$_REQUEST['empnumeroprocesso'] = str_replace("-","", $_REQUEST['empnumeroprocesso']);
		$wheremun[] = "p.pronumeroprocesso ilike '%".$_REQUEST['empnumeroprocesso']."%'";
		$whereest[] = "p.pronumeroprocesso ilike '%".$_REQUEST['empnumeroprocesso']."%'";
	}
	
    if($_REQUEST['preid']) {
        $wheremun[] = " pre.preid = '{$_REQUEST['preid']}' ";
        $whereest[] = " pre.preid = '{$_REQUEST['preid']}' ";
    }

    if($_REQUEST['obrid']) {
        $wheremun[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
        $whereest[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
    }
    
	if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				$whereest[] = "1=2";
				break;
			case 'Estadual':
				$wheremun[] = "1=2";
				break;
		}
	}
	
	if($_REQUEST['municipio']) {
		$wheremun[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		$whereest[] = "1=2";
	}
	if($_REQUEST['estuf']) {
		$wheremun[] = "m.estuf='".$_REQUEST['estuf']."'";
		$whereest[] = "p.estuf='".$_REQUEST['estuf']."'";
	}
	
	if($_REQUEST['tipoobra'][0]) {
            $wheremun[] = "pre.ptoid IN (".implode(',', $_REQUEST['tipoobra']).")";
	}
	
	if($_REQUEST['resid']) {
		$wheremun[] = "r.resid = '".$_REQUEST['resid']."'";
		$whereest[] = "r.resid = '".$_REQUEST['resid']."'";
	}
	
	if($_REQUEST['usuario']){
		$wheremun[] = "u.usucpf = '".$_REQUEST['usuario']."'";
		$whereest[] = "u.usucpf = '".$_REQUEST['usuario']."'";
	}
	
	if($_REQUEST['termogerado'] != "3") {
		if($_REQUEST['termogerado'] == "1") {
			//$inner = "INNER JOIN par.termocompromissopac t ON t.muncod = m.mucndo";
			$wheremun[] = "( SELECT 
                                                coalesce(count(t.terid), 0)
                                        FROM par.termocompromissopac t
                                        WHERE t.muncod = m.muncod 
                                            AND t.terstatus = 'A'
                                            AND t.proid = p.proid 
                                            AND t.tpdcod = 102 OR t.tpdcod = 103
                                        ) > 0";
			$whereest[] = "( SELECT 
                                                coalesce(count(t.terid), 0)
                                        FROM par.termocompromissopac t
                                        WHERE t.estuf = e.estuf
                                            AND t.terstatus = 'A'
                                            AND t.proid = p.proid 
                                            AND t.tpdcod = 102 OR t.tpdcod = 103
                                        ) > 0";
		} else {
			$wheremun[] = "( SELECT 
                                                coalesce(count(t.terid), 0)
                                        FROM par.termocompromissopac t
                                        WHERE t.muncod = m.muncod 
                                            AND t.terstatus = 'A'
                                            AND t.proid = p.proid 
                                            AND t.tpdcod = 102 OR t.tpdcod = 103
                                        ) = 0";
			$whereest[] = "( SELECT 
                                                coalesce(count(t.terid), 0)
                                        FROM par.termocompromissopac t
                                        WHERE t.estuf = e.estuf 
                                            AND t.terstatus = 'A'
                                            AND t.proid = p.proid 
                                            AND t.tpdcod = 102 OR t.tpdcod = 103
                                        ) = 0";	
		}
	}
	if($_REQUEST['assinado'] != "3") {
		if($_REQUEST['assinado'] == "1") {
			$wheremun[] = "ter.terassinado = TRUE";
			$whereest[] = "ter.terassinado = TRUE";
		}
		if($_REQUEST['assinado'] == "2") {
			$wheremun[] = "( ter.terassinado = FALSE OR ter.terassinado IS NULL )";
			$whereest[] = "( ter.terassinado = FALSE OR ter.terassinado IS NULL )";
		}
	}
	
	if($_REQUEST['anexopublicacao'] != "3") {
		if($_REQUEST['anexopublicacao'] == "1") { // sim
			
			$wheremun[] = " p.proid  in (select proid from par.processoobraanexo)";
			$whereest[] = " p.proid  in (select proid from par.processoobraanexo)";
		}
		if($_REQUEST['anexopublicacao'] == "2") { // não
			
			$wheremun[] = " p.proid not in (select proid from par.processoobraanexo)";
			$whereest[] = " p.proid not in (select proid from par.processoobraanexo)";
		}
	}

    $pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        // Negando estadual
        $whereest[] = "1=2";
        $wheremun[] = " p.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        // Negando estadual
        $whereest[] = "1=2";
        $wheremun[] = " p.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        // Negando municipal
        $wheremun[] = "1=2";
        $whereest[] = " p.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        // Negando municipal
        $wheremun[] = "1=2";
        $whereest[] = " p.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }
    
    if($_REQUEST['ternumero']){
    	$wheremun[]= "ter.ternumero = {$_REQUEST['ternumero']}";
    	$whereest[]= "ter.ternumero = {$_REQUEST['ternumero']}";
    }
	
//	if($_REQUEST['pendenciaObras'] == '1'){ // com pendencias
//		$arrObrasPendentesM = array();
//		$arrObrasPendentesE = array();
//		$arrObrasPendentesM = bloqueioObra('','','','Municipal');
//		$arrObrasPendentesE = bloqueioObra('','','','Estadual');
//		$wheremun[] = " p.muncod in ('".implode("','", $arrObrasPendentesM)."')";
//		$whereest[] = " p.estuf in ('".implode("','", $arrObrasPendentesE)."')";
//	}
//
//	if($_REQUEST['pendenciaObras'] == '2'){ // sem pendencias
//		$arrObrasPendentesM = array();
//		$arrObrasPendentesE = array();
//		$arrObrasPendentesM = bloqueioObra('','','','Municipal');
//		$arrObrasPendentesE = bloqueioObra('','','','Estadual');
//		$wheremun[] = " p.muncod not in ('".implode("','", $arrObrasPendentesM)."')";
//		$whereest[] = " p.estuf not in ('".implode("','", $arrObrasPendentesE)."')";
//	}
	
	
}

if($exportaXLS){
	$cabecalho = array("Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Assinado","Anexo de publicação", "Qtd obras no processo","Tipo de Assinatura", "Usuário de Criação");
//	$cabecalho = array("Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Qtd obras no processo");
} else {
	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Assinado","Anexo de publicação","Qtd obras no processo","Tipo de Assinatura");
//	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Qtd obras no processo");
}


// Montando SQL para buscar os processo Municipais

$sqlMun = "SELECT distinct";

if(!$exportaXLS){
	$sqlMun .= "
		'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaTermos2(\''||m.muncod||'\', false, \''|| p.proid ||'\',this);\"></center>' as mais,
		CASE WHEN (SELECT COUNT(terid) FROM par.termocompromissopac WHERE proid = p.proid and terstatus='A') = 0 THEN '<center><img src=../imagens/editar_nome_vermelho.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/gerarTermoObra&acao=A&proid='|| p.proid ||'&muncod=' || m.muncod ||'\',\'Termo\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\">
		</center>' ELSE '' END as acoes, 
	";
}

if ($exportaXLS) {
    $strProcessoMun = "(to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')) as pronumeroprocesso,";
    $municipio 		= "m.mundescricao as mundescricao,";
}else {
    $municipio 		= "'<a onclick=\"abrePlanoTrabalho(''municipio'', '''|| iu.mun_estuf ||''', '''|| iu.muncod ||''', '''|| iu.inuid ||''')\" style=cursor:pointer; >'||m.mundescricao||'</a>' as mundescricao,";
    $strProcessoMun = "(
                    CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.pronumeroprocesso) > 0 THEN
                        '<span class=\"processoDetalhes processo_detalhe\" >' || to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                    ELSE
                        '<span class=\"processo_detalhe\" >' || to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                    END
                ) || '</span>' as pronumeroprocesso,";  
}
$sqlMun .= "
                $strProcessoMun
	   	COALESCE(r.resdescricao,'N/A') as resdescricao,
	   	{$municipio} 
	   	m.estuf, 
	   	CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
		( 
		SELECT sum(round(po4.prevalorobra,2)) 
		FROM (
			SELECT DISTINCT po.*  
			FROM obras.preobra po 
			INNER JOIN par.empenhoobra eo ON eo.preid = po.preid 
			INNER JOIN par.empenho emp ON emp.empid = eo.empid and empcodigoespecie not in ('03', '13')  
			INNER JOIN workflow.documento d ON d.docid=po.docid 
			WHERE 
				emp.empnumeroprocesso=p.pronumeroprocesso
				AND po.prestatus='A' 
				AND d.esdid IN ( 228, 360, 365, 366, 367, 754, 683, 755 )
			) as po4 
		) as valortotal,			   
		(SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,";


if($exportaXLS){
	
	$sqlproComArquivos = "select proid from par.processoobraanexo";
	$result = $db->carregar($sqlproComArquivos);
	foreach($result as $k => $v )
	{
		$arrayProid[] = $v['proid']; 
	}
	$strProid = implode($arrayProid, ', ');
	
	
	$sqlMun .= "CASE WHEN te.terid IS NULL THEN
	   	 	'Não criado' 
	   	 ELSE 
	   	 	CASE WHEN te.terassinado=TRUE THEN
	   	 		'Sim' 
	   	 	ELSE 
	   	 		'Não' 
	   	 	END 
	   	 END as assinado,
	   	 CASE WHEN p.proid in ({$strProid}) THEN
	   	 	'Sim'
	   	 ELSE
	   	 	'Não'
	   	 END
	   	 as anexopublicacao,
	   	 ";
} else {
	$sqlMun .= "CASE WHEN te.terid IS NULL THEN 
	   	 	'<center>Não criado</center>' 
	   	 ELSE 
	   	 	'<center><input type=checkbox '||CASE WHEN ((SELECT count(em.empnumeroprocesso) FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid AND pg.pagstatus = 'A' WHERE em.empnumeroprocesso=p.pronumeroprocesso and empcodigoespecie not in ('03', '13')))>0 THEN 
	   	 		'disabled' 
	   	 	ELSE 
	   	 		'' 
	   	 	END||' name=termo value='||te.terid||' onclick=assinarTermo(this); '||CASE WHEN te.terassinado=TRUE THEN
	   	 		'checked' 
	   	 	ELSE 
	   	 		'' 
	   	 	END ||'></center>'
	   	 END as assinado,";
}

if(!$exportaXLS){
    $sqlMun .= "CASE WHEN te.terassinado=TRUE THEN '<center><img onclick=\"enviarAnexo2('||p.proid||');\" style=\"cursor:pointer;\" src=\"../imagens/reject.png\"></center>' ELSE '<center>Termo não assinado</center>' END as anexopublicado,";
}

$sqlMun .= "(select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                                        pc.pocstatus = 'A' and po.proid = p.proid) as qtdobrempenhadas,
			CASE WHEN ter.terassinado = 't' AND ter.terdataassinatura IS NULL THEN 'Validada Manualmente' WHEN ter.terassinado = 't' AND ter.terdataassinatura IS NOT NULL THEN 'Validada Digitalmente' ELSE '-' END as tipovalidacao ";

if($exportaXLS){
 	$sqlMun .= ",u.usunome ";
}

$sqlMun .= "
    FROM 
        par.processoobra p
	INNER JOIN par.instrumentounidade iu ON iu.muncod = p.muncod

        LEFT JOIN par.processoobraspaccomposicao poc ON(poc.proid = p.proid) and poc.pocstatus = 'A'
        LEFT JOIN par.termocompromissopac te ON te.proid=p.proid AND te.terstatus = 'A'
        LEFT JOIN obras.preobra pre ON(
            pre.preid = poc.preid 
            AND pre.prestatus = 'A')

        LEFT JOIN obras2.obras obr
        INNER JOIN workflow.documento doc2 ON(
            doc2.docid = obr.docid
            AND tpdid = ".TPDID_OBJETO.")
        INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
            ON( obr.obrid = pre.obrid
            AND obridpai IS NULL)

		LEFT JOIN par.resolucao r ON r.resid=p.resid
		INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
		LEFT JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' and (ter.tpdcod = 102 or ter.tpdcod = 103)
		LEFT JOIN seguranca.usuario 	    u ON u.usucpf  = ter.usucpf 
		WHERE 1=1
		and p.prostatus = 'A'
	".(($wheremun)?"AND ".implode(" AND ", $wheremun):"")."";

//ver(simec_htmlentities($sqlMun),d);

// Montando SQL para buscar os processo Estaduais

$sqlEst = "SELECT distinct ";

if(!$exportaXLS){
	$sqlEst .= "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaTermos2(false, \''||p.estuf||'\', \''|| p.proid ||'\',this);\"></center>' as mais,
			CASE WHEN (SELECT COUNT(terid) FROM par.termocompromissopac WHERE proid = p.proid and terstatus='A') = 0 THEN '<center><img src=../imagens/editar_nome_vermelho.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/gerarTermoObra&acao=A&proid='|| p.proid ||'&estuf=' || p.estuf ||'\',\'Termo\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\">
			</center>' ELSE '' END as acoes, ";
}

if ($exportaXLS) {
    $strProcessoEst = "to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as pronumeroprocesso,";
    $estuf			= "iu.estuf as estuf,";
}else {
    $strProcessoEst = "'<span class=\"processo_detalhe\" >' || to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso,";  
    $estuf			= "'<a onclick=\"abrePlanoTrabalho(''estado'', '''||iu.estuf||''', '''','''|| iu.inuid ||''')\" style=cursor:pointer; >'||iu.estuf||'</a>' as estuf,";
}

$sqlEst .= " $strProcessoEst
		   	COALESCE(r.resdescricao,'N/A') as resdescricao,
		   	'' as mundescricao, 
		   	 {$estuf}
		   	CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
			( 
			SELECT sum(round(po4.prevalorobra,2)) 
			FROM (
				SELECT DISTINCT po.*  
				FROM obras.preobra po 
				INNER JOIN par.empenhoobra eo ON eo.preid = po.preid 
				INNER JOIN par.empenho emp ON emp.empid = eo.empid and empcodigoespecie not in ('03', '13')  
				INNER JOIN workflow.documento d ON d.docid=po.docid 
				WHERE 
					emp.empnumeroprocesso=p.pronumeroprocesso
					AND po.prestatus='A' 
					AND d.esdid IN ( 228, 360, 365, 366, 367, 754, 683, 755 )
				) as po4 
			) as valortotal,			   
			(SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,";

if($exportaXLS){
	$sqlEst .= "CASE WHEN ter.terid IS NULL THEN 
	   	 	'Não criado' 
	   	 ELSE 
	   	 	CASE WHEN ter.terassinado=TRUE THEN
	   	 		'Sim' 
	   	 	ELSE 
	   	 		'Não' 
	   	 	END 
	   	 END as assinado,
	   	 	 CASE WHEN p.proid in ({$strProid}) THEN
	   	 	'Sim'
	   	 ELSE
	   	 	'Não'
	   	 END
	   	 as anexopublicacao,
	   	 ";
} else {
	$sqlEst .= "CASE WHEN te.terid IS NULL THEN 
	   	 	'<center>Não criado</center>' 
	   	 ELSE 
	   	 	'<center><input type=checkbox '||CASE WHEN ((SELECT count(em.empnumeroprocesso) FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid AND pg.pagstatus = 'A' WHERE em.empnumeroprocesso=p.pronumeroprocesso and empcodigoespecie not in ('03', '13')))>0 THEN 
	   	 		'disabled' 
	   	 	ELSE 
	   	 		'' 
	   	 	END||' name=termo value='||te.terid||' onclick=assinarTermo(this); '||CASE WHEN te.terassinado=TRUE THEN
	   	 		'checked' 
	   	 	ELSE 
	   	 		'' 
	   	 	END ||'></center>'
	   	 END as assinado,";
}

if(!$exportaXLS){
 	$sqlEst .= "CASE WHEN ter.terassinado=TRUE THEN '<center><img onclick=\"enviarAnexo2('||p.proid||');\" style=\"cursor:pointer;\" src=\"../imagens/reject.png\"></center>' ELSE '<center>Termo não assinado</center>' END as anexopublicado,";
}


$sqlEst .= "(select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                                        pc.pocstatus = 'A' and po.proid = p.proid) as qtdobrempenhadas,
			CASE WHEN ter.terassinado = 't' AND ter.terdataassinatura IS NULL THEN 'Validada Manualmente' WHEN ter.terassinado = 't' AND ter.terdataassinatura IS NOT NULL THEN 'Validada Digitalmente' ELSE '-' END as tipovalidacao ";

if($exportaXLS){
 	$sqlEst .= ",u.usunome ";
}

$sqlEst .= "FROM 
				par.processoobra p
        	INNER JOIN par.instrumentounidade iu ON iu.estuf = p.estuf

                LEFT JOIN par.processoobrasparcomposicao poc ON(poc.proid = p.proid)  and poc.pocstatus = 'A'
                LEFT JOIN par.termocompromissopac te ON te.proid=p.proid AND te.terstatus = 'A'
                LEFT JOIN obras.preobra pre ON(
                    pre.preid = poc.preid 
                    AND pre.prestatus = 'A')

                LEFT JOIN obras2.obras obr
                INNER JOIN workflow.documento doc2 ON(
                    doc2.docid = obr.docid
                    AND tpdid = ".TPDID_OBJETO.")
                INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
                    ON( obr.obrid = pre.obrid
                    AND obridpai IS NULL)

			LEFT JOIN par.resolucao r ON r.resid=p.resid
			LEFT JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' and (ter.tpdcod = 102 or ter.tpdcod = 103)
			LEFT JOIN seguranca.usuario 	    u ON u.usucpf  = ter.usucpf
			INNER JOIN territorios.estado e ON e.estuf=p.estuf
			WHERE 
				1=1
				and prostatus = 'A'
				".(($whereest)?"AND ".implode(" AND ", $whereest):"")."";

$sql = $sqlMun." UNION ALL (".$sqlEst.")";
    if($exportaXLS) {
        ob_clean();
        header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
        header ( "Pragma: no-cache" );
        header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Description: MID Gera excel" );
        $db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
        exit;
    } else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
//			ver(simec_htmlentities($sql),d);
            $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
        }
    }
?>
<script>
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
</script>
