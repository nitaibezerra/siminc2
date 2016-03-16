<?php
echo montaAbasTOR();

echo cabecalhoTOR( 'Análise', $msg );

function carregarAnaliseSubacao(){
	
	global $db;
	
	// PAR
	$sql = "
        SELECT DISTINCT
            CASE WHEN s.sbaid  IS NOT NULL THEN
                '<center>
                    <img style=\"cursor:pointer\" src=\"/imagens/mais.gif\" class=abre border=0
                         sbdid='|| sd.sbdid ||' />
                </center>'
            END as acao,
            s.sbadsc
        FROM
        par.subacao s
        INNER JOIN par.subacaodetalhe 			sd  ON sd.sbaid = s.sbaid
        INNER JOIN par.termocomposicao 			tc  ON tc.sbdid = sd.sbdid
        INNER JOIN par.subacaoitenscomposicao 	si  ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
        INNER JOIN par.anexotorpar 				atp ON atp.atpid = si.atpid
        INNER JOIN workflow.documento 			doc ON doc.docid = atp.docid
        WHERE
            tc.dopid = {$_SESSION['dopid']}
        ORDER BY
            s.sbadsc
    ";

	$cabecalho = array( $marcarTdos, "Descrição da Subação" );
	$db->monta_lista_simples($sql,$cabecalho,5000,0,'N','90%','N');

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

	$('.analisar').click(function(){

		if( $('#todos').attr('checked') ){
			$('[name="docids[]"]').attr('checked',$('#todos').attr('checked'));
		}
		
		var arrIcoId = '';
		if( $('[name="docids[]"]::checked').length < 1 ){
			alert('Favor selecionar pelo menos 1(um) item para compor o parecer.');
			return false;
		}

		$('#req').val('analisar');
		$('#formAnalise').submit();
	});

	$('[name="docids[]"]').live('click',function(){
		if(!$(this).attr('checked')){
			$('#todos').attr('checked',false);
		}
	});

	$('#todos').click(function(){
		$('[name="docids[]"]').attr('checked',$(this).attr('checked'));
	});
    
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
			   		data: "req=carregaTermos&sbdid="+sbdid,
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
<form method="post" name="formAnalise" id="formAnalise">
	<?php carregarAnaliseSubacao(); ?>
	<input type="hidden" name=req id=req />
	<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
		<tr>
			<td class="subTituloDireita" width="30%">
				Parecer:
			</td>
			<td width="100%" colspan="3" align="left">
				<?=campo_textarea('parecer', 'S', 'S', 'Descreva o parecer.', '150', '6', '5000') ?>
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita" width="30%">
				Ação:
			</td>
			<td width="100%" colspan="3" align="left">
				<?php
				$sql = "SELECT 
							p.pflcod 
						FROM 
							seguranca.perfilusuario pu 
						INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod 
						WHERE 
							pu.usucpf = '".$_SESSION['usucpf']."' 
							AND p.pflstatus = 'A' 
							AND p.sisid = ".$_SESSION['sisid'];
							
				$perfil = $db->carregarColuna($sql);
				
				$sql = "SELECT DISTINCT
							aed.aedid as codigo,
							aed.aeddscrealizar as descricao  
						FROM 	
							workflow.acaoestadodoc aed 
						INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
						WHERE 
							aedstatus = 'A' 
							AND aed.aedid NOT IN (53)
							AND aed.esdidorigem = 1096 --  ".( $perfil && !$db->testa_superuser() ? " AND pflcod in (".implode(',',$perfil).")" : "" )." 
						ORDER  BY 
							2";
				
				$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
				?>
			</td>
		</tr>
		<tr>
 			<td width="100%" colspan="3" align="center"> 
 				<?php if( testaPermissaoAnaliseTOR() ){?>
				<label class="TituloTela" style="color:#000000;"><input type="button" class=analisar value="Gravar Análise"></label>
				<?php }?>
 			</td> 
		</tr>
	</table>
</form>