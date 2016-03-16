<?
function excluir(){
    global  $db;
    if( $_REQUEST['pcmid'] != '' ){
        
        $sql = "UPDATE academico.procampo SET pcmstatus = 'I' WHERE pcmid = ".$_POST['pcmid'];
        $db->executar($sql);
        $db->commit();
?>
        <script>
            alert('Campus Excluido');
            window.location.href = window.location.href;
        </script>
<?
        
    }
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

monta_titulo('PROCAMPO', '');
?>	
<style>
.imgbot{
	cursor:pointer;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(document).ready(function(){
	$('.inserir').click(function(){
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaPROCAMPO&acao=A', 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	});
	$('.excluir').click(function(){
        $('#req').val('excluir');
        $('#pcmid').val( $(this).attr('id') );
        $('#formulario').submit();
    });
	$('.alterar').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaPROCAMPO&acao=A&pcmid='+id, 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="" name="req" id="req" />
	<input type="hidden" value="" name="pcmid" id="pcmid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
 			<td>
 			<? 
 			$sql = "SELECT
 						'<center><img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" class=\"alterar imgbot\" id=\"'|| exp.pcmid ||'\">&nbsp;'||
 						'<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" class=\"excluir imgbot\" id=\"'|| exp.pcmid ||'\"></center>' as acao,
						entnome,
						mundescricao||' - '||mun.estuf as municipio,
						'<center>'||pcmnumvagas||'</center>' as pcmnumvagas,
						'<center>'||pcmprevisto||'</center>' as pcmprevisto,
						'<center>'||pcmrealizado||'</center>' as pcmrealizado
					FROM
						academico.procampo exp
					INNER JOIN entidade.entidade e ON e.entid = exp.entidcampus
					LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid
					LEFT  JOIN territorios.municipio mun ON mun.muncod = ed.muncod
					WHERE
						entidunidade = '{$_SESSION['academico']['entid']}'
						AND pcmstatus = 'A'";;
 			
 			$cabecalho = array("Ação","Campus", "Município", "N° de Vagas", "Previsto", "Realizado" );
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
 			?>
	        </td>
	    </tr>
		<tr>
 			<td>
	        	<input type="button" class="inserir" value="Adicionar Novo">
	        </td>
	    </tr>    
	</table>
</form>
