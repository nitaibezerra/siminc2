<?
function excluir(){
    global  $db;
    if( $_REQUEST['esmid'] != '' ){
        
        $sql = "UPDATE academico.expensinomedico SET esmstatus = 'I' WHERE esmid = ".$_POST['esmid'];
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

monta_titulo('Plano de Expansão do Ensino Médico em Regiões Prioritárias - Curso Existentes e Novos', '');
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
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaPlanoExpEnsMed&acao=A', 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	});
	$('.excluir').click(function(){
        $('#req').val('excluir');
        $('#esmid').val( $(this).attr('id') );
        $('#formulario').submit();
    });
	$('.alterar').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaPlanoExpEnsMed&acao=A&esmid='+id, 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="" name="req" id="req" />
	<input type="hidden" value="" name="esmid" id="esmid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
 			<td>
 			<? 
 			$sql = "SELECT
 						'<center><img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" class=\"alterar imgbot\" id=\"'|| exp.esmid ||'\">&nbsp;'||
 						'<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" class=\"excluir imgbot\" id=\"'|| exp.esmid ||'\"></center>' as acao,
						entnome,
						mundescricao||' - '||mun.estuf as municipio,
						CASE WHEN esmtipo = 'N'
							THEN 'Curso Novo'
							ELSE 'Expansão'
						END as tipo,
						'<center>'||esmnumvagas||'</center>' as esmnumvagas,
						'<center>'||esmprevisto||'</center>' as esmprevisto,
						'<center>'||esmrealizado||'</center>' as esmrealizado
					FROM
						academico.expensinomedico exp
					INNER JOIN entidade.entidade e ON e.entid = exp.entidcampus
					LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid
					LEFT  JOIN territorios.municipio mun ON mun.muncod = ed.muncod
					WHERE
						entidunidade = '{$_SESSION['academico']['entid']}'
						AND esmstatus = 'A'";;
 			
 			$cabecalho = array("Ação","Campus", "Município", "Expansão/Curso Novo", "N° de Vagas", "Previsto", "Realizado" );
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
 			?>
	        </td>
	    </tr>
		<tr>
 			<td>
	        	<input type="button" class="inserir" value="Adicionar Campus">
	        </td>
	    </tr>    
	</table>
</form>


