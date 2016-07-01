<?php

if( !$_GET['sosid'] ){
	echo "<script>
			alert('Não foi possível encontrar as informações mínimas necessárias para acessar essa tela.');
			self.close();
		</script>";
	exit;
}else
	$sosid = $_GET['sosid'];

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

$habil 		  	= true;
$somenteLeitura = 'S';
if ( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC) ) == false 
	|| true ){ // por enquanto somente bloqueia o que nao pode nunca
	$habil 		  	= false;
	$somenteLeitura = 'N';
}

switch ( $_POST['op'] ){
	case 'salvar':
		break;
	case 'apagar':
		$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOS();
		if( $ProrrogacaoPrazoOs->apagaProrrogacao( $_POST['prposid'] ) )
			exit( "Prorrogação apagada om sucesso!" );
		else
			exit( "Erro ao apagar prorrogação." );
		break;
}

$osObra  = new Supervisao_Os_Obra();
$arEmpid = $osObra->listaEmpidPorOs( $sosid );

// busca prorrogacao
$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOS();
if( $ProrrogacaoPrazoOs->verificaProrrogacao( $sosid ) )
	$prorrogacao = 1;


//Chamada de programa
// include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";
// criaAbaOS();
monta_titulo( 'Histórico de Prorrogação de Prazo', '' );

//$somenteLeitura = 'S';
?>
<meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>.link{cursor:pointer}</style>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<tr>
		<td class="SubtituloCentro" colspan="2">
		Prorrogações de Prazos
		</td>	
	</tr>
	<tr>
		<td colspan="4">
			<div id="divListaObra" style="height: 300px; overflow: auto;">
		
		<table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_obra">
		
				<thead>
                	<tr style="background-color: #CDCDCD;">
                		<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Ação
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		OS Nº
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Prazo anterior
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Novo prazo
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Justificativa
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Usuário
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Data da prorrogação
                        </th>
					</tr>
				</thead>
		<?php	
				$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOs();
				$listaProrrogacoes = $ProrrogacaoPrazoOs->listaProrrogacoesDeOS( $sosid );
				
				if( $listaProrrogacoes ){

				$i = 0;
                    
				foreach ( $listaProrrogacoes as $dadoProrrogacao ){
					$color = ($i%2 ? '#FFFFFF' : '#FFFFFF');
					$i++;
		?>	
                	<tr style="background-color: <?=$color?>;">
                    	<td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<a onclick="javascript:apagaProrrogacao(<?=$dadoProrrogacao['prposid']?>)" href="#"><img title="Apagar Prorrogação" src="../imagens/excluir.gif"/></a>
                        </td>
                    	<td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoProrrogacao['sosnum'] ?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=formata_data($dadoProrrogacao['prazo_anterior'])?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=formata_data($dadoProrrogacao['novo_prazo'])?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<a href="#" onmouseover="javascript:jQuery('#linha<?=$dadoProrrogacao['prposid']?>').css('display','')" onmouseout="javascript:jQuery('#linha<?=$dadoProrrogacao['prposid']?>').css('display','none')">Ver justificativa</a>
                    		<div id="linha<?=$dadoProrrogacao['prposid']?>" style="position:absolute;display:none;border:1px solid #000;background:#fff;max-width:300px;padding:5px;"><?=$dadoProrrogacao['justificativa'] ?></div>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoProrrogacao['usunome'] ?>
                        </td>
                        <td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=formata_data($dadoProrrogacao['data_prorrogacao'])?>
                        </td>
					</tr>
				
		<?php			
				}
			}else{
		?>
				<tr style="color: red;">
					<td colspan="6">
						Nenhuma prorrogação encontrada.
					</td>
				</tr>
			<?php } ?>

			</table>
			</div>
		</td>	
	</tr>
	<tr>
		<td style="text-align:center;" colspan="2">
		<?php if( $_GET['l'] ){ ?>
			<input type="button" value="Voltar" onclick="javascript:history.back()"/>
		<?php } ?>
		<input type="button" value="Fechar" onclick="javascript:self.close()"/></td>
	</tr>
	
</table>
</form>
<script>
// function enviaFormulario(){

// 	$('#formulario_os').submit();
	
// }
function apagaProrrogacao( prposid ){
	if( confirm("Deseja realmente apagar esse registro Prorrogação?") ){
		jQuery.ajax({
			url: window.location.href,
			type:"POST",
			data:{op:'apagar',prposid:prposid},
			success: function( retorno ){
				alert( retorno );
				window.location.reload();
			}
		});
	}
}
</script>
