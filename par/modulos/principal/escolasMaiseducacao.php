<?php 

$inuid = $_SESSION['par']['inuid'];

if( $_REQUEST['requisicaoDblinkAjax'] ){
	$_REQUEST['requisicaoDblinkAjax']($_REQUEST);
	die();
}

/* listarEscolasMaiseducacao
 * Função testa se a unidade possui pendências no Mais Educação
* Deve ser passado um Array() da seguinte maneira:
* Ex.: Array( 'campo' => 'estuf', 'valor' => 'AC' );
* */
function listarEscolasMaiseducacao(){

	global $db;
	
	$inuid = $_SESSION['par']['inuid'];
	$arrEsfera = pegaArrEsferaInuid( $inuid );

	$sql = "SELECT
				*
			FROM
				dblink(
					'".DBLINK_PARAM_PDEINTERATIVO."',
					'SELECT DISTINCT
						lp.pdicodinep as inep,
						lp.pdenome as nome,
						CASE WHEN est.esddsc IS NULL THEN ''Não Iniciado'' ELSE est.esddsc END as situacao
					FROM 
						pddeinterativo.listapdeinterativo lp
					INNER JOIN pdeescola.pddemepriorizadas 	prio on prio.entcodent = lp.pdicodinep
					INNER JOIN pdeescola.memaiseducacao 	me 	 on lp.pdicodinep = me.entcodent AND me.memanoreferencia = 2014 and memstatus = ''A''
					LEFT JOIN workflow.documento 			doc  on doc.docid = me.docid
					LEFT JOIN workflow.estadodocumento 		est  on est.esdid = doc.esdid
					WHERE 
						lp.{$arrEsfera['campo']} = ''{$arrEsfera['valor']}''
						AND ( ( doc.esdid is null ) OR ( doc.esdid = 32 ) ) order by lp.pdenome'
				) as  rs (
					inep text, nome text, situacao text
				) 
			ORDER BY
				nome";

	$result = $db->carregar( $sql );
	
	require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
	$ajax = new MontaListaAjax($db);
	$cabecalho = array("Codigo INEP", "Nome da Escola", "Situação");
	
	echo '<span style="margin: 20px; color:red; font-size: 10pt;"> Foram encontradas <strong>'. count( $result ).'</strong> Escolas ainda Não Iniciadas ou Em Cadastramento no Mais Educação </span> <br />';
	
	echo $ajax->montaLista($result,$cabecalho,15,5,"N","center",100);
	
	die;
}

$arrPerfil = pegaPerfilGeral();

if( in_array(PAR_PERFIL_PREFEITO ,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO ,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO ,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_ESTADUAL ,$arrPerfil) 
	|| in_array( PAR_PERFIL_EQUIPE_MUNICIPAL,$arrPerfil) 
	|| in_array( PAR_PERFIL_PARLAMENTAR ,$arrPerfil) 
	|| in_array( PAR_PERFIL_ENTIDADE_EXECUTORA,$arrPerfil) 
	|| in_array( PAR_PERFIL_UNIVERSIDADE_ESTADUAL,$arrPerfil)
	|| in_array( PAR_PERFIL_CONSULTA_ESTADUAL,$arrPerfil)
    || in_array( PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$arrPerfil)
	|| in_array( PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil)
    || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)
	|| in_array( PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO,$arrPerfil)
	|| in_array( PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE,$arrPerfil)
)
{
	
	echo "<input type='hidden' id='popupaviso' value='1'>" ;
	
	
}
else
{
	echo "<input type='hidden' id='popupaviso' value='0'> ";
}
if( $inuid ){
	
	$arrEsfera = pegaArrEsferaInuid( $inuid );
	
	if( verificaPendenciaMaiseducacao( $arrEsfera ) ){
?>

		<style>
		.ui-widget-header
{
    background: #357ebd !important;
    background-image: none;
    color: Black;
}
		
		</style>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
		<script>
		
		jQuery(document).ready(function(){

			if(jQuery('#popupaviso').val() === '1')
			{
				html = '<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">'+
							'<tr>'+
								'<td align="justify" style="font-family: Calibri; font-size: 12pt;">'+
									'Prezado Secretário,<br><br>'+
		
									'Ajude as escolas de sua rede a participar do PROGRAMA MAIS EDUCAÇÃO.<br>'+ 
									'Prazo de adesão ao Programa Mais Educação estendido. Faça a adesão de sua escola até o dia 30 de junho no sítio <a style=cursor:pointer href=http://pdeinterativo.mec.gov.br/ >http://pdeinterativo.mec.gov.br</a>'+
									'<br /><br />Confira <a href="#"style="font-weight: bold; color: #357ebd;" id="aqui">aqui</a> como está o processo de adesão das escolas da sua rede.'+
									' Relembramos que a escola deve preencher o seu Plano de atendimento escolar, salvar e enviar à secretaria de educação.  A secretaria deve então enviar o plano ao MEC.<br><br>'+
		
									'<a style=cursor:pointer href=http://portal.mec.gov.br/index.php?option=com_content&view=article&id=16690&Itemid=1113 >'+
									'Saiba Mais<br>'+
									'</a>'+
								'</td>'+
							'</tr>'+
						'</table>';				   		
					jQuery( "#div_dialog_workflow" ).html(html);		
					jQuery( '#div_dialog_workflow' ).show();		
					jQuery( "#div_dialog_workflow" ).dialog({
						resizable: true,
						width: 700,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						title: "Programa - Mais Educação",
						buttons: {
							"OK": function() {
								jQuery( this ).dialog( "close" );
							},
						}
					});
			}
				
				jQuery('#aqui').click( function(){
// 					jQuery('#dialog').dialog('close');
					jQuery.ajax({
				   		type: 	"POST",
				   		url: 	window.location,
				   		data: 	'requisicaoDblinkAjax=listarEscolasMaisEducacao',
				   		async: 	false,
				   		success: function(msg){
				   		
				   			var html = msg;
							
				   			jQuery( "#div_dialog_workflow" ).html(html);		
				   			jQuery( '#div_dialog_workflow' ).show();		
				   			jQuery( "#div_dialog_workflow" ).dialog({
								resizable: true,
								width: 800,
								modal: true,
								show: { effect: 'drop', direction: "up" },
								buttons: {
									"Fechar": function() {
										jQuery( this ).dialog( "close" );
									},
								}
							});
				   		}
					});
				});
		});
		</script>
		<center>
			<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >
			<div style="margin-top:250px; align:center;">
				<img border="0" title="Aguardando" src="../imagens/carregando.gif">Carregando...</div>
			</div>
		</center>
		<div id="dialog_maiseducacao" style="display:none"></div>
<?php 		
	}
}
?>