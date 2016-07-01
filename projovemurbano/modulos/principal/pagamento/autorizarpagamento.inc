<script language="javascript" type="text/javascript">

jQuery(document).ready(function() {
	
    jQuery('.inserir').click(function(){
    	
        if( jQuery('[name="perid[]"]:checked').length < 1 ){
            alert('Seleciona pelo menos um periodo.');
            return false;
        }
        if(confirm('Você realmente deseja enviar para pagamento os itens selecionados?')){
        	jQuery('#requisicao').val('autorizaPagamentos');
        	jQuery('#form').submit();
        }
    });
    
    jQuery('#marcar-todos').live('click',function(){
        jQuery('.doc').attr('checked', jQuery(this).attr('checked'));
    });
})
</script>
<?
// <input type="checkbox" id="marcar-todos" >
$check = '<input type="hidden" value="" name="requisicao" id="requisicao" />';

$sql = "
		SELECT DISTINCT
			'<center><input type=\"checkbox\" class=\"doc\" name=\"perid[]\" value=\"'|| per.perid ||'\" /></center>' as marcar,
			per.perperiodo,
			(
			SELECT DISTINCT
				SUM(dia.diabolsaspendentes)
			FROM  projovemurbano.diario dia
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE
				dia.perid = per.perid
			) as pendente,
			(
			SELECT DISTINCT
				SUM(dia.diabolsasrecusadas)
			FROM
			projovemurbano.diario dia
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE
				dia.perid = per.perid
			)as rejeitadas
		FROM
		projovemurbano.diario dia
		INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
		WHERE
			per.perid in (
							SELECT
								perid
							FROM
								projovemurbano.periodocurso
							WHERE
								ppuid = {$_SESSION['projovemurbano']['ppuid']})
		AND perdtinicio <= now()
		GROUP BY
			per.perperiodo,
			per.perid
		ORDER BY
			per.perperiodo
			
		";
		
		$cabecalho = array("$check","Período","Bolsas em Estado Pendente","Bolsas Rejeitadas");
		$alinhamento	= array( 'center', 'center','center', 'center','center', 'center');
		
		$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', 'form','',$alinhamento);
?>
    <table>
	    <tr>
		    <td align="left">
				<input type="button" class="inserir" value="Inserir">
	        </td>
	    </tr>
    </table>
