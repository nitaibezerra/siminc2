<script language="javascript" type="text/javascript">

jQuery(document).ready(function() {
	
    jQuery('.bloquear').click(function(){
    	if(confirm('Você realmente deseja bloquear esse aluno?')){
	        jQuery('#caecpf option').attr('selected',true);
	        jQuery('#requisicao').val('bloquearPagamento');
	        jQuery('#form').submit();
    	}
    });

})

function desbloquearPagamento(caecpf) {
	if(confirm('Você realmente deseja desbloquear esse aluno?')){
		 jQuery('#caecpf_desbloqueia').val(caecpf);
	     jQuery('#requisicao').val('desbloquearPagamento');
	     jQuery('#form').submit();
	}
}
</script>
<form id="form" enctype="multipart/form-data" name="form" method="POST" >
    <input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="" name="caecpf_desbloqueia" id="caecpf_desbloqueia" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
		<tr>
			<td class="SubTituloDireita" width="25%">CPF:</td>
			<td width="75%">
				<? echo campo_texto('caecpf', "N","S", "CPF", 16, 140, "[###.###.###-##,]", "", '', '', 0, 'id="caecpf" required', '', '', ');' ); ?>
			</td>
		</tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" class="bloquear" value="Bloquear">
            </td>
        </tr>
    </table>
</form>
<?
monta_titulo('Alunos Bloqueados Para Receber Bolsa de Estudos','');
	$sql = "SELECT DISTINCT
				'<img src=../imagens/desbloquear.gif id=\"'|| cae.caecpf ||'\" value=\"'||cae.caecpf||'\" onclick=\"desbloquearPagamento('||cae.caecpf||')\"  style=\"cursor: pointer;\">' as acao,
				cae.caecpf,
				cae.caenome,
				CASE WHEN pju.muncod IS NULL
					THEN 'Estadual'
					ELSE 'Municipal'
				END as esfera,
				COALESCE(pju.estuf,mun.estuf) as estuf,
				coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
				COALESCE('POLO '||cae.polid,'Não Possui') as polo,
				'NÚCLEO '||nuc.nucid as nucleo,
				tur.turdesc
			FROM 
					   projovemurbano.cadastroestudante cae
			INNER JOIN projovemurbano.turma tur              ON cae.turid = tur.turid
			INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid 
			INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
			LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
			LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
			WHERE 
				cae.caebloquearpagamento is true
			ORDER BY 
				esfera,
				estuf, 
				mundescricao, 
				polo, 
				nucleo";

    $cabecalho = array("Desbloquear","CPF","Nome","Esfera","UF","Município","Polo","Nucleo","Turma");
    $alinhamento= array( 'center', 'center','center', 'center','center', 'center','center', 'center','center');    
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', '','',$alinhamento);
?>
