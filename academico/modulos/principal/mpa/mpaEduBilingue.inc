<?
function excluir(){
    global  $db;
    if( $_REQUEST['edbid'] != '' ){
        
        $sql = "UPDATE academico.educacaobilingue SET edbstatus = 'I' WHERE edbid = ".$_POST['edbid'];
        $db->executar($sql);
        $db->commit();
?>
        <script>
            alert('Curso Excluido');
            window.location.href = window.location.href;
        </script>
<?
        
    }
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

monta_titulo('Educação Bilíngue', '');
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
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaEduBilingue&acao=A', 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	});
	$('.excluir').click(function(){
        $('#req').val('excluir');
        $('#edbid').val( $(this).attr('id') );
        $('#formulario').submit();
    });
	$('.alterar').click(function(){
		var id = $(this).attr('id');
		var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaEduBilingue&acao=A&edbid='+id, 'adiciona_campus', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
    });
});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="" name="req" id="req" />
	<input type="hidden" value="" name="edbid" id="edbid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
 			<td>
 			<? 
 			$sql = "SELECT
 						'<center><img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" class=\"alterar imgbot\" id=\"'|| exp.edbid ||'\">&nbsp;'||
 						'<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" class=\"excluir imgbot\" id=\"'|| exp.edbid ||'\"></center>' as acao,
 						'<center>'||cebdsc||'</center>' as cebdsc,
						'<center>'||edbnumvagas||'</center>' as edbnumvagas,
 						'<center>'||
 						CASE 
 							WHEN edbmodalidade = 'p' THEN 'Presencial'
 							ELSE 'A Distância'
 						END ||'</center>' as edbmodalidade,
						'<center>'||edbprevisto||'</center>' as edbprevisto,
						'<center>'||edbrealizado||'</center>' as edbrealizado
					FROM
						academico.educacaobilingue exp 
					INNER JOIN academico.cursoeducbilingue ceb ON ceb.cebid = exp.cebid
					INNER JOIN entidade.entidade e ON e.entid = exp.entidunidade
					LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid
					LEFT  JOIN territorios.municipio mun ON mun.muncod = ed.muncod
					WHERE
						entidunidade = '{$_SESSION['academico']['entid']}'
						AND edbstatus = 'A'";;
 			
 			$cabecalho = array("Ação","Curso","Vagas","Modalidade", "Previsto", "Realizado" );
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


