<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>


function inserirCustos_s(orcid) {
	var param='&alpid='+jQuery('#alpid').val();
	if(orcid!='') {
		param += '&orcid='+orcid;
	}


	
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}


function carregarListaCustos_s(alpid) {
	ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listacustos_s');
}


function carregarListaCustos() {
	ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_listacustos');
	ajaxatualizar('requisicao=calcularCustoAlunoCusteio&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_custoaluno');
}

function carregarNaturezaDespesasCustos() {
	ajaxatualizar('requisicao=carregarNaturezaDespesasCustos&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','td_naturezadespesas');
}

function excluirCustos_s(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+jQuery('#alpid').val(),'div_listacustos_s');
	}

}

function solicitarAlteracaoOrcamento() {

	var conf = confirm('Deseja realmente solicitar alteração no orçamento?');

	if(!conf) {
		return false;
	}

	jQuery('#inserircustos_s').css('display','');
	jQuery('#confirmarsolicitacao_orcamento').css('display','');
	jQuery('#cancelarsolicitacao_orcamento').css('display','');
	jQuery('#tbl_situacaoanterior_orcamento').css('display','none');
		

	var alpid = 0;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=solicitarAlteracaoProjetoOrcamento&sifid=<?=$_SESSION['sisfor']['sifid'] ?>',
   		async: false,
   		success: function(r){alpid=r;}
	});
	jQuery('#alpid').val(alpid);
	carregarListaCustos_s(alpid);

	jQuery("#modalSolicitacaoOrcamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}


function solicitarAlteracaoBolsas() {

	var conf = confirm('Deseja realmente solicitar mundanças no quantitativos de bolsas?');

	if(!conf) {
		return false;
	}

	jQuery('#confirmarsolicitacao_bolsas').css('display','');
	jQuery('#cancelarsolicitacao_bolsas').css('display','');
	jQuery('#div_listabolsas_anterior').html('');
	jQuery('#tbl_situacaoanterior_bolsas').css('display','none');
		

	var alpid = 0;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=solicitarAlteracaoProjetoBolsas&sifid=<?=$_SESSION['sisfor']['sifid'] ?>',
   		async: false,
   		success: function(r){alpid=r;}
	});
	jQuery('#alpid').val(alpid);
	carregarListaBolsas_s(alpid);

	jQuery("#modalSolicitacaoBolsas").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}


function solicitarAlteracaoMeta() {

	var conf = confirm('Deseja realmente solicitar mundança na meta do curso?');

	if(!conf) {
		return false;
	}

	jQuery('#confirmarsolicitacao_meta').css('display','');
	jQuery('#cancelarsolicitacao_meta').css('display','');
	jQuery('#div_listameta_anterior').html('');
	jQuery('#tbl_situacaoanterior_meta').css('display','none');
		

	var alpid = 0;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=solicitarAlteracaoProjetoMeta&sifid=<?=$_SESSION['sisfor']['sifid'] ?>',
   		async: false,
   		success: function(r){alpid=r;}
	});
	jQuery('#alpid').val(alpid);
	carregarMeta_s(alpid);

	jQuery("#modalSolicitacaoMeta").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function solicitarAlteracaoVigencia() {

	var conf = confirm('Deseja realmente solicitar mundança na vigência do curso?');

	if(!conf) {
		return false;
	}

	jQuery('#confirmarsolicitacao_vigencia').css('display','');
	jQuery('#cancelarsolicitacao_vigencia').css('display','');
	jQuery('#div_listavigencia_anterior').html('');
	jQuery('#tbl_situacaoanterior_vigencia').css('display','none');
		

	var alpid = 0;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=solicitarAlteracaoProjetoVigencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>',
   		async: false,
   		success: function(r){alpid=r;}
	});
	jQuery('#alpid').val(alpid);
	carregarVigencia_s(alpid);

	jQuery("#modalSolicitacaoVigencia").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function carregarListaBolsas_s(alpid) {
	ajaxatualizar('requisicao=carregarQuantitativoPorPerfil&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_carregarQuantitativoPorPerfil');
}

function carregarMeta_s(alpid) {
	ajaxatualizar('requisicao=carregarMeta_s&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_meta');
}

function carregarVigencia_s(alpid) {
	ajaxatualizar('requisicao=carregarVigencia_s&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_vigencia');
}


function exibirSolicitacaoAlteracaoOrcamento(alpid, alpsituacao) {

	jQuery('#inserircustos_s').css('display','none');
	jQuery('#confirmarsolicitacao_orcamento').css('display','none');
	jQuery('#cancelarsolicitacao_orcamento').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=orcamento&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listacustos_anterior');
	jQuery('#tbl_situacaoanterior_orcamento').css('display','');
	
	carregarListaCustos_s(alpid);

	jQuery("#modalSolicitacaoOrcamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function exibirSolicitacaoAlteracaoBolsas(alpid, alpsituacao) {

	jQuery('#confirmarsolicitacao_bolsas').css('display','none');
	jQuery('#cancelarsolicitacao_bolsas').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=bolsas&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listabolsas_anterior');
	jQuery('#tbl_situacaoanterior_bolsas').css('display','');
	
	ajaxatualizar('requisicao=carregarQuantitativoPorPerfil&consulta=1&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_carregarQuantitativoPorPerfil');

	jQuery("#modalSolicitacaoBolsas").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function exibirSolicitacaoAlteracaoMeta(alpid, alpsituacao) {

	jQuery('#confirmarsolicitacao_meta').css('display','none');
	jQuery('#cancelarsolicitacao_meta').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=meta&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listameta_anterior');
	jQuery('#tbl_situacaoanterior_meta').css('display','');
	
	ajaxatualizar('requisicao=carregarMeta_s&consulta=1&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_meta');

	jQuery("#modalSolicitacaoMeta").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}


function exibirSolicitacaoAlteracaoVigencia(alpid, alpsituacao) {

	jQuery('#confirmarsolicitacao_vigencia').css('display','none');
	jQuery('#cancelarsolicitacao_vigencia').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=vigencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listavigencia_anterior');
	jQuery('#tbl_situacaoanterior_vigencia').css('display','');
	
	ajaxatualizar('requisicao=carregarVigencia_s&consulta=1&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'td_vigencia');

	jQuery("#modalSolicitacaoVigencia").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function autorizarAlteracao(alpid, alpautorizado) {

	switch(alpautorizado) {
		case '2':
			var conf = confirm('Deseja realmente autorizar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpautorizado=2&alpid='+alpid,'');
			
			break;

		case '3':
			var conf = confirm('Deseja realmente recusar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpautorizado=3&alpid='+alpid,'');
			
			break;
		
	}

	window.location=window.location;
	
	
}

function gravarBolsas_s() {
	ajaxatualizar('requisicao=inserirQtdEquipeIES_s&alpid='+jQuery('#alpid').val()+'&'+jQuery("[name^='epivalor_s[']").serialize()+'&'+jQuery("[name^='epiqtd_s[']").serialize(),'modalExibir');
}

function gravarMeta_s() {
	ajaxatualizar('requisicao=gravarMeta_s&alpid='+jQuery('#alpid').val()+'&sifprofmagisterio_s='+jQuery('#sifprofmagisterio_s').val(),'');
}

function gravarVigencia_s() {
	ajaxatualizar('requisicao=gravarVigencia_s&alpid='+jQuery('#alpid').val()+'&sifvigenciadtini_s='+jQuery('#sifvigenciadtini_s').val()+'&sifvigenciadtfim_s='+jQuery('#sifvigenciadtfim_s').val(),'');
}



function confirmarSolicitacao() {
	var alertar = jQuery('#modalExibir').html();

	if(alertar!='') {
		alert(alertar);
		return false;
	}
	
	var pt = prompt("Escreva uma justificativa para solicitação de alteração do projeto:", "");

	if(pt==null || pt=='') {
		return false;
	}
	ajaxatualizar('requisicao=confirmarSolicitacao&alpautorizado=1&alpid='+jQuery('#alpid').val()+'&alpjustificativa='+pt,'');
	window.location=window.location;
}

function alertarConfirmacao(tipo) {
	var associativeArray = {};
	associativeArray['orcamento'] = 'Alertamos que somente alguns programas/cursos permitem o remanejamento orçamentário.  Ajuste de recursos de custeio, normalmente, está ligado ao ajuste de meta. Deseja continuar?';
	associativeArray['bolsas'] = 'Alertamos que somente alguns programas/cursos permitem o remanejamento de bolsa entre funções. Ajuste de bolsa, normalmente, está ligado ao ajuste de meta, uma vez que as funções formadoras estão diretamente ligadas ao quantitativo de cursistas. Deseja continuar?';
	associativeArray['meta'] = 'Alertamos que ajuste de meta, normalmente, implica ajustes no quantitativo de bolsas e recursos de custeio, uma vez que os mesmos estão diretamente ligados ao quantitativo de cursistas.  Sendo assim, deve-se planejar de forma conjunta os ajustes nesses itens, quando for necessário. Deseja continuar?';
	associativeArray['vigencia'] = 'Alertamos que as solicitações de prorrogação de projeto são limitadas. É necessário planejar para que esta solicitação seja adequada à conclusão deste projeto, alterando apenas da data de término da vigência do mesmo. Deseja continuar?';

	var conf = confirm(associativeArray[tipo]);

	return conf;
		
}


function preencherTotais() {

	var totalqtd = 0;
	jQuery("[name^='epiqtd_s[']").each(function() {
		if(jQuery(this).val()!='') {
			totalqtd += parseFloat(jQuery(this).val());
		}
	});
	
	jQuery('#epiqtdtotal_s').val(totalqtd);

	jQuery("[name^='dadosperfil_s[]']").each(function() {
		var totalporperfil = 0;
		var valorun = parseFloat(jQuery(this).val());

		var qtd = 0;
		if(jQuery('#epiqtd_s_'+jQuery(this).attr('id')).val()!='') {
			var qtd = parseFloat(jQuery('#epiqtd_s_'+jQuery(this).attr('id')).val());
		}

		totalporperfil = qtd*valorun;

		jQuery('#epivalor_s_'+jQuery(this).attr('id')).val(mascaraglobal('###.###.###,##',totalporperfil.toFixed(2)));

	});

	var totalvalor = 0;
	jQuery("[name^='epivalor_s[']").each(function() {
		if(jQuery(this).val()!='') {
			var valor = replaceAll(replaceAll(jQuery(this).val(),".",""),",",".");
			totalvalor += parseFloat(valor);
		}
	});

	jQuery('#epivalortotal_s').val(mascaraglobal('###.###.###,##',totalvalor.toFixed(2)));
	
}

</script>

<div id="modalExibir" style="display:none;"></div>

<div id="modalSolicitacaoOrcamento" style="display:none;">

<input type="hidden" name="alpid" id="alpid" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Destinada ao remanejamento orçamentário, quando permitido pelo programa/curso e para adequações de orçamento em caso de ajuste de meta..</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos_s" onclick="inserirCustos_s('');"></p>
		<div id="div_listacustos_s"></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_orcamento" onclick="if(alertarConfirmacao('orcamento')){confirmarSolicitacao();}"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_orcamento" onclick="jQuery('#modalSolicitacaoOrcamento').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_orcamento" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<div id="div_listacustos_anterior"></div>
		</td>
	</tr>

</table>

</div>

<div id="modalSolicitacaoBolsas" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Destinada ao remanejamento de bolsas, quando permitido pelo programa/curso e para adequações de bolsa em caso de ajuste de meta.</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td id="td_carregarQuantitativoPorPerfil"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_bolsas" onclick="gravarBolsas_s();if(alertarConfirmacao('bolsas')){confirmarSolicitacao();}"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_bolsas" onclick="jQuery('#modalSolicitacaoBolsas').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_bolsas" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td>
		<div id="div_listabolsas_anterior"></div>
		</td>
	</tr>

</table>

</div>


<div id="modalSolicitacaoMeta" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Para o caso da necessidade de se ajustar a meta pactuada. Cabe esclarecer que ajuste de meta, comumente, gera ajustes em orçamento e bolsas e, nestes casos, todos os ajustes deverão ser solicitados conjuntamente.</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td id="td_meta"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_meta" onclick="gravarMeta_s();if(alertarConfirmacao('meta')){confirmarSolicitacao();}"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_meta" onclick="jQuery('#modalSolicitacaoMeta').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_meta" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td>
		<div id="div_listameta_anterior"></div>
		</td>
	</tr>

</table>

</div>


<div id="modalSolicitacaoVigencia" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Destinada a alteração da vigência pactuada no projeto, ou seja, são as solicitações de prorrogação de prazos para a execução do projeto, alterando apenas da data de término da vigência do mesmo</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Vigência</td>
		<td id="td_vigencia"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_vigencia" onclick="gravarVigencia_s();if(alertarConfirmacao('vigencia')){confirmarSolicitacao();}"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_vigencia" onclick="jQuery('#modalSolicitacaoVigencia').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_vigencia" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Vigência</td>
		<td>
		<div id="div_listavigencia_anterior"></div>
		</td>
	</tr>

</table>

</div>


<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Alterar projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>
		<p>Prezados(as) Coordenadores(as),</p>
		<p>Visando permitir a adequação dos projetos às necessidades de execução, estão disponíveis quatro tipos de alterações. Alertamos para o devido planejamento das necessidades de alterações uma vez que as mesmas serão limitadas.</p>
		<p>
		<b>- vigência:</b> destinada a alteração da vigência pactuada no projeto, ou seja, são as solicitações de prorrogação de prazos para a execução do projeto, alterando apenas da data de término da vigência do mesmo;<br>
		<b>- meta:</b> para o caso da necessidade de se ajustar a meta pactuada. Cabe esclarecer que ajuste de meta, comumente, gera ajustes em orçamento e bolsas e, nestes casos, todos os ajustes deverão ser solicitados conjuntamente;<br>
		<b>- bolsa:</b> destinada ao remanejamento de bolsas, quando permitido pelo programa/curso e para adequações de bolsa em caso de ajuste de meta.<br>
		<b>- orçamento:</b> destinada ao remanejamento orçamentário, quando permitido pelo programa/curso e para adequações de orçamento em caso de ajuste de meta.
		</p>
		
		
		
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<p align="center"><b>Solicitações de alteração de orçamento</b></p>
		
		<? 
		
		if($db->testa_superuser()) {
			$param = "<img align=absmiddle src=../imagens/valida1.gif style=cursor:pointer; onclick=\"autorizarAlteracao('||a.alpid||',\'2\');\"> <img align=absmiddle src=../imagens/valida3.gif style=cursor:pointer;  onclick=\"autorizarAlteracao('||a.alpid||',\'3\');\">";
		}
		
		$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"exibirSolicitacaoAlteracaoOrcamento('||a.alpid||','||a.alpautorizado||')\">' as acao, 
					   '<span style=font-size:x-small;>'||replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )</span>' as solicitacao,
					   '<span style=font-size:x-small;>'||replace(to_char(u2.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u2.usunome||' ( '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )</span>' as autorizacao,
					   '<div style=width:450px;height:60px;overflow:auto;>'||coalesce(alpjustificativa,'-')||'</span>' as alpjustificativa,
					   '<span style=font-size:x-small;>'||CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
															   WHEN alpautorizado='2' THEN 'AUTORIZADO'
															   WHEN alpautorizado='3' THEN 'RECUSADO' END||'</span>' as situacao
				FROM sisfor.alterarprojeto a 
				INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
				WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND a.alptipo='orcamento' AND alpautorizado IS NOT NULL
				ORDER BY a.alpdtsolicitou";
		
		$cabecalho = array("&nbsp;", "Usuário que solicitou", "Usuário que autorizou", "Justificativa", "Situação");
		$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
		
		
		?>
		
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloEsquerda">
		<input type="button" value="Solicitar Alteração de Orçamento" onclick="solicitarAlteracaoOrcamento();">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<p align="center"><b>Solicitações de alteração de bolsas</b></p>
		
		<? 
		
		if($db->testa_superuser()) {
			$param = "<img align=absmiddle src=../imagens/valida1.gif style=cursor:pointer; onclick=\"autorizarAlteracao('||a.alpid||',\'2\');\"> <img align=absmiddle src=../imagens/valida3.gif style=cursor:pointer;  onclick=\"autorizarAlteracao('||a.alpid||',\'3\');\">";
		}
		
		$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"exibirSolicitacaoAlteracaoBolsas('||a.alpid||','||a.alpautorizado||')\">' as acao, 
					   replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )' as solicitacao,
					   replace(to_char(u2.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u2.usunome||' ( '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )' as autorizacao,
					   coalesce(alpjustificativa,'-'),
					   CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
							WHEN alpautorizado='2' THEN 'AUTORIZADO'
							WHEN alpautorizado='3' THEN 'RECUSADO' END as situacao
				FROM sisfor.alterarprojeto a 
				INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
				WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND a.alptipo='bolsas' AND alpautorizado IS NOT NULL
				ORDER BY a.alpdtsolicitou";
		
		$cabecalho = array("&nbsp;", "Usuário que solicitou", "Usuário que autorizou", "Justificativa", "Situação");
		$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
		
		
		?>
		
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloEsquerda">
		<input type="button" value="Solicitar Alteração de Bolsas" onclick="solicitarAlteracaoBolsas();">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<p align="center"><b>Solicitações de alteração de Meta</b></p>
		
		<? 
		
		if($db->testa_superuser()) {
			$param = "<img align=absmiddle src=../imagens/valida1.gif style=cursor:pointer; onclick=\"autorizarAlteracao('||a.alpid||',\'2\');\"> <img align=absmiddle src=../imagens/valida3.gif style=cursor:pointer;  onclick=\"autorizarAlteracao('||a.alpid||',\'3\');\">";
		}
		
		$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"exibirSolicitacaoAlteracaoMeta('||a.alpid||','||a.alpautorizado||')\">' as acao, 
					   replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )' as solicitacao,
					   replace(to_char(u2.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u2.usunome||' ( '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )' as autorizacao,
					   coalesce(alpjustificativa,'-'),
					   CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
							WHEN alpautorizado='2' THEN 'AUTORIZADO'
							WHEN alpautorizado='3' THEN 'RECUSADO' END as situacao
				FROM sisfor.alterarprojeto a 
				INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
				WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND a.alptipo='meta' AND alpautorizado IS NOT NULL
				ORDER BY a.alpdtsolicitou";
		
		$cabecalho = array("&nbsp;", "Usuário que solicitou", "Usuário que autorizou", "Justificativa", "Situação");
		$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
		
		
		?>
		
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloEsquerda">
		<input type="button" value="Solicitar Alteração de Meta" onclick="solicitarAlteracaoMeta();">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<p align="center"><b>Solicitações de alteração de Vigência do curso</b></p>
		
		<? 
		
		if($db->testa_superuser()) {
			$param = "<img align=absmiddle src=../imagens/valida1.gif style=cursor:pointer; onclick=\"autorizarAlteracao('||a.alpid||',\'2\');\"> <img align=absmiddle src=../imagens/valida3.gif style=cursor:pointer;  onclick=\"autorizarAlteracao('||a.alpid||',\'3\');\">";
		}
		
		$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"exibirSolicitacaoAlteracaoVigencia('||a.alpid||','||a.alpautorizado||')\">' as acao, 
					   replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )' as solicitacao,
					   replace(to_char(u2.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u2.usunome||' ( '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )' as autorizacao,
					   coalesce(alpjustificativa,'-'),
					   CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
							WHEN alpautorizado='2' THEN 'AUTORIZADO'
							WHEN alpautorizado='3' THEN 'RECUSADO' END as situacao
				FROM sisfor.alterarprojeto a 
				INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
				WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND a.alptipo='vigencia' AND alpautorizado IS NOT NULL
				ORDER BY a.alpdtsolicitou";
		
		$cabecalho = array("&nbsp;", "Usuário que solicitou", "Usuário que autorizou", "Justificativa", "Situação");
		$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'N');
		
		
		?>
		
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloEsquerda">
		<input type="button" value="Solicitar Alteração da vigência do curso" onclick="solicitarAlteracaoVigencia();">
		</td>
	</tr>
</table>
</form>