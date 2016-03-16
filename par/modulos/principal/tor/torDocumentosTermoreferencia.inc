<?php
echo montaAbasTOR();

echo cabecalhoTOR( 'Termos de Referência', $msg );
echo "<p style='color:#F00; text-indent:15px'>Obs.: Itens que são de pregão não precisam anexar Termo de Referência.</p>";

function carregaDadoSubacao(){
	
	global $db;
	
	// PAR
	$sql = "SELECT DISTINCT  
				CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
					'<center>
						<img style=\"cursor:pointer\" src=\"/imagens/mais.gif\" class=abre border=0
							 sbdid='|| sd.sbdid ||' />
					</center>'
				END as acao,  
				s.sbadsc
			FROM 
				par.subacao s
			INNER JOIN par.subacaodetalhe 			sd  ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.subacaoitenscomposicao 		ico ON ico.sbaid = sd.sbaid AND ico.icoano = sd.sbdano
			INNER JOIN par.termocomposicao 			tc  ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar 			dp  ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			INNER JOIN par.processopar 			prp ON prp.prpid = dp.prpid AND prp.prpstatus = 'A'
			WHERE 
				dp.dopid = {$_SESSION['dopid']}
				AND ico.picid NOT IN (
					SELECT DISTINCT
						picid
					FROM 
						par.propostaitemcomposicao
					WHERE
						picpregao IS TRUE
					)";

	$cabecalho = array( $marcarTdos, "Descrição da Subação" );
	$db->monta_lista($sql,$cabecalho,20,5,'N','95%','S');

}
?>
<style type="">
    .ui-dialog-titlebar{
    text-align: center;
    }
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">

$(document).ready(function(){

	$('.abre').live('click',function(){

		$('#aguardando').show();
		
		var sbdid = $(this).attr('sbdid');
		var linha = $(this);
		var html = '';
		
		if( $(linha).attr('src') == '/imagens/mais.gif' ){

			$(linha).attr('src','/imagens/menos.gif');
			
			if( $('#tr_'+sbdid).css('display') == 'none' ){
				$('#tr_'+sbdid).show();
			}else{
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "req=carregaItensTOR&sbdid="+sbdid,
			   		async: false,
			   		success: function(msg){
				   		html = '<tr id=tr_'+sbdid+' bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">'+
				   					'<td align="left" title="&nbsp;Ações&nbsp;" colspan=2>'+msg+
				   					'</td>'+
				   				'</tr>';
			   			$(linha).parent().parent().parent().after(html);
			   			$('#aguardando').hide();
			   		}
				});
			}
		}else{
			$(linha).attr('src','/imagens/mais.gif');
			$('#tr_'+sbdid).hide();
		}
		$('#aguardando').hide();
	});

	$('.anexar').click(function(){

		var arrIcoId = '';
		$('[name="icoids[]"]').each(function() {
		   if( this.checked)
		   {
			   arrIcoId =  arrIcoId + $(this).val() + '|';
		   }
		});

		if( arrIcoId != '' ){
			window.open('par.php?modulo=principal/tor/popupAnexoTOR&acao=A&arrIcoId=' + arrIcoId,'','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
		}else{
			alert('Selecione pelo menos 1(um) Item.');
		}
	});

	$('[name="icoids[]"]').live('click',function(){
		if(!$(this).attr('checked')){
			var sbdid = $(this).attr('class');
			$('#'+sbdid).attr('checked',false);
		}
	});
	
	$('.todos').live('click',function(){
		var sbdid = $(this).attr('id');
		$('.'+sbdid).attr('checked',$(this).attr('checked'));
	}); 
});

function voltar()
{
	location.href='par.php?modulo=principal/administracaoDocumentos&acao=A';
}

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

</script>
<?php 
carregaDadoSubacao(); 

$arrPflsTR = Array( PAR_PERFIL_SUPER_USUARIO, 
					PAR_PERFIL_ADMINISTRADOR,
 
					PAR_PERFIL_PREFEITO,
					PAR_PERFIL_EQUIPE_MUNICIPAL,
					PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
					
					PAR_PERFIL_EQUIPE_ESTADUAL,
					PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,
					PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO );

if( possui_perfil( $arrPflsTR ) ){
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">
			<input type="button" class=anexar value="Anexar TR"></label>
		</td>
	</tr>
</table>
<?php 
}
?>