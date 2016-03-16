<?
monta_titulo( "Equipamentos - Infraestrutura", "&nbsp;");
?>
<script type="text/javascript">

	function salvarDistorcaoInfraestruturaEquipamentos(local)
	{
		jQuery("[name^='num_bom['][class!='disabled']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});
		jQuery("[name^='num_ruim['][class!='disabled']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});

		
		if(!verificaRespostasPerguntas()){
			alert('Favor responder todas as perguntas!');
			erro = 1;
			return false;
		}
				
		var num = jQuery("[name^='necessaria['][class!='disabled'][value='']").length;
		if(num != 0){
			erro = 1;
			alert('Favor informar qual seria a quantidade ideal para atender às necessidades da escola!');
			return false;
		}
		
		if(num == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("#form_distorcao_infraestrutura_equipamentos").submit();
		}
	}
	
	function calculaEquipamento(rieid)
	{
		var numA = jQuery("[name='num_bom[" + rieid + "]']").val();
		var numI = jQuery("[name='num_ruim[" + rieid + "]']").val();
		
		numA = numA.replace(".","");
		numA = numA.replace(".","");
		numA = numA.replace(".","");
		numA = numA*1;
		
		numI = numI.replace(".","");
		numI = numI.replace(".","");
		numI = numI.replace(".","");
		numI = numI*1;
		
		if(!numA){
			numA = 0;
		}
		if(!numI){
			numI = 0;
		}
		
		var total = numA + numI;
		
		jQuery("#total_" + rieid).html( mascaraglobal('[.###]',total) );
		
	}
	
	function ativaQtdeNecessaria(rieid)
	{
		
		var total = jQuery("[id='total_" + rieid + "']").html();
		if(!total){
			jQuery("[name='atende_" + rieid + "']").attr("checked","");
			alert('Favor informar a quantidade de equipamentos!');
			return false;
		}
		jQuery("[name='necessaria[" + rieid + "]']").attr("readonly","");
		jQuery("[name='necessaria[" + rieid + "]']").attr("class","normal");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onblur","MouseBlur(this)");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onmouseout","MouseOut(this)");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onfocus","MouseClick(this);this.select()");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onmouseover","MouseOver(this)");
	}
	
	function desativaQtdeNecessaria(rieid)
	{
		var total = jQuery("[id='total_" + rieid + "']").html();
		if(!total){
			jQuery("[name='atende_" + rieid + "']").attr("checked","");
			alert('Favor informar a quantidade de equipamentos!');
			return false;
		}
		jQuery("[name='necessaria[" + rieid + "]']").attr("readonly","readonly");
		jQuery("[name='necessaria[" + rieid + "]']").val("");
		jQuery("[name='necessaria[" + rieid + "]']").attr("class","disabled");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onblur","");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onmouseout","");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onfocus","");
		jQuery("[name='necessaria[" + rieid + "]']").attr("onmouseover","");
	}
	
	function verificaValorTotal(tmeid)
	{
		var total = jQuery("[id='total_" + rieid + "']").html();
		total = total*1;
		var necessario = jQuery("[name='necessaria[" + rieid + "]']").val();
		if(total >= necessario){
			alert('Favor informar um valor maior que o total!');
			jQuery("[name='necessaria[" + rieid + "]']").val("");
		}
	}
	
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0"	align="center">
<tr>
	<td class="blue center" width="20%" >Orientações</td>
	<td class="blue" >
		<p>Nesta etapa, vamos identificar a dispoinibilidade de materiais e equipamentos informando a quantidade de cada item existente na escola, de acordo com o estado de conservação dos mesmos. Ou seja, quantas unidades de cada item indicado na tabela abaixo estão em bom estado de conservação e quantas unidades estão em estado de conservação ruim. Para este diagnóstico, considera-se como "Bom" o material ou equipamento que pode ser utilizado ao longo de, no mínimo, dois anos letivos. "Ruim", portanto, signfica que o estado de conservação daquele material ou equipamento não permite a sua utilização durante, pelo menos, dois anos letivos.</p>
		<p>Depois de conhecer o estado de conservação dos materiais e equipamentos existentes, é importante saber se a escola considera a quantidade daquele item suficiente para as suas necessidades. Por exemplo: a escola possui uma geladeira (mesmo que o seu estado de conservação seja ruim), mas avalia que, para atender ao dia-a-dia, seria necessária mais uma geladeira. Neste caso, a quantidade ideal que será indicada é "2" (duas).</p>
		<p>Por fim, no quadro de perguntas, assinale a resposta correspondente  à situação que mais se aproxima da realidade da escola.</p>
	</td>
</tr>
</table>
<?php $arrCategoriaMateriais = recuperaCategoriaMateriaisEquipamentos() ?>
<?php $arrDados = recuperaInstalacoesEquipamentos() ?>
<form name="form_distorcao_infraestrutura_equipamentos" id="form_distorcao_infraestrutura_equipamentos" method="post" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoInfraestruturaEquipamentos" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita">Materias e Equipamentos</td>
		<td>
			<?php if($arrCategoriaMateriais): ?>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
					<thead>
						<tr>
							<td class="bold center" ></td>
							<td class="bold center" colspan="2" >Estado de Conservação</td>
							<td class="bold center" colspan="3" >Quantidade</td>
						</tr>
						<tr>
							<td class="bold center" >Materias e Equipamentos</td>
							<td class="bold center" >Bom</td>
							<td class="bold center" >Ruim</td>
							<td class="bold center" >Total existente</td>
							<td class="bold center" >O total existente atende às<br /> necessidades da escola?</td>
							<td class="bold center" >Qual seria a quantidade ideal para<br /> atender às necessidades da escola?</td>
						</tr>
					</thead>
				<?php $c=1;$x=1;foreach($arrCategoriaMateriais as $cat): ?>
					<?php $arrMateriais = recuperaMateriaisEquipamentos($cat['cmeid']) ?>
					<?php $cor = $x%2 == 1 ? "#FFFFFF" : "" ?>
					<tr bgcolor="<?php echo $cor ?>">
						<td class="esquerda bold" ><?php echo $c ?> - <?php echo $cat['cmedesc'] ?></td>
						<td class="esquerda" ></td>
						<td class="esquerda" ></td>
						<td class="esquerda" ></td>
						<td class="esquerda" ></td>
						<td class="esquerda" ></td>
					</tr>
					<?php $m=1;$x++;foreach($arrMateriais as $mat): ?>
						<?php $cor = $x%2 == 1 ? "#FFFFFF" : "" ?>
						<tr bgcolor="<?php echo $cor ?>">
							<td class="esquerda" ><?php echo $cat['cmeid'] ?>.<?php echo $m ?> - <?php echo $mat['tmedesc'] ?></td>
							<td class="center" ><?php echo campo_texto("num_bom[".$mat['tmeid']."]","N","S","","10","20","[.###]","","","","","","calculaEquipamento('{$mat['tmeid']}')",($arrDados[$mat['tmeid']]['bom'] != "" ? number_format($arrDados[$mat['tmeid']]['bom'],"",2,".") : "0") ) ?></td>
							<td class="center" ><?php echo campo_texto("num_ruim[".$mat['tmeid']."]","N","S","","10","20","[.###]","","","","","","calculaEquipamento('{$mat['tmeid']}')",($arrDados[$mat['tmeid']]['ruim'] != "" ? number_format($arrDados[$mat['tmeid']]['ruim'],"",2,".") : "0") ) ?></td>
							<td id="total_<?php echo $mat['tmeid'] ?>" class="center" ><?php echo $arrDados[$mat['tmeid']]['total'] ? number_format($arrDados[$mat['tmeid']]['total'],"",2,".") : "0" ?></td>
							<td class="center" >
								<input type="radio" <?php echo $arrDados[$mat['tmeid']]['atende'] == "S" ? "checked='checked'" : "" ?> onclick="desativaQtdeNecessaria('<?php echo $mat['tmeid'] ?>')" name="atende_<?php echo $mat['tmeid'] ?>" value="S" />Sim 
								<input type="radio" <?php echo $arrDados[$mat['tmeid']]['atende'] == "N" ? "checked='checked'" : "" ?> onclick="ativaQtdeNecessaria('<?php echo $mat['tmeid'] ?>')" name="atende_<?php echo $mat['tmeid'] ?>" value="N" />Não  
							</td>
							<td class="center" ><?php echo campo_texto("necessaria[".$mat['tmeid']."]","N",($arrDados[$mat['tmeid']]['atende'] == "N" ? "S" : "N"),"","10","20","[.###]","","","","","","", ( $arrDados[$mat['tmeid']]['necessita'] ? number_format($arrDados[$mat['tmeid']]['necessita'],"",2,".") : "")) ?></td>
						</tr>
					<?php $m++;$x++;endforeach; ?>
				<?php $c++;endforeach; ?>
				</table>
			<?php endif; ?>
		</td>
	</tr>
	<tr id="tr_perguntas" >
		<td class="SubTituloDireita" width="20%">Perguntas</td>
		<td>
			<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'I' and prgsubmodulo = 'E' and prgstatus = 'A'"; ?>
			<?php quadroPerguntas($sql) ?>
		</td>
	</tr>
	<tr id="tr_navegacao" >
		<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
		<td>
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_1_instalacoes'" >
			<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDistorcaoInfraestruturaEquipamentos()">
			<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarDistorcaoInfraestruturaEquipamentos('C');">
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese'" >
		</td>
	</tr>
	</table>
</form>