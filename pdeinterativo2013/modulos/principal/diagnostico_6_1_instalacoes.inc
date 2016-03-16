<?php monta_titulo( "Instalações - Infraestrutura", "&nbsp;"); ?>

<script type="text/javascript" language="javascript">
	function salvarDistorcaoInfraestruturaInstalacao(local)
	{
		
		var erro = 0;
	
		var msg = '';
		jQuery("[name^='num_adequado['][class!='disabled']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});
		jQuery("[name^='num_inadequado['][class!='disabled']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});
		
		jQuery("[name^='total['][class!='disabled']").each(function(k, v){
			
			var numA = parseInt( jQuery("[name='num_adequado[" + this.id + "]']").val());
			var numI = parseInt( jQuery("[name='num_inadequado[" + this.id + "]']").val());			
			var soma = numA + numI;
			
			if (v.value != soma )
			{
				msg += jQuery("[id='item_"+this.id+"']").html() + '\n' ;
			}
		;});
		
		if (msg != ''){
			alert ("Favor verificar a soma da(s) dependência(s): \n\n" + msg);
			erro = 1;
			return false;
		}
		
		
		var num = jQuery("[name^='justificativa['][class!='disabled'][value='']").length;
		if(num != 0){
			alert('Favor informar por quê exite(m) instalação(ões) inadequada(s)!');
			erro = 1;
			return false;
		}
		
		if(!verificaRespostasPerguntas()){
			alert('Favor responder todas as perguntas!');
			erro = 1;
			return false;
		}
		
		if( !jQuery("[id='rdo_espaco_sim']:checked").val() && !jQuery("[id='rdo_espaco_nao']:checked").val() ){
			alert('Favor informar se existem espaços públicos ou privados na escola!');
			erro = 1;
			return false;
		}
				
		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("#form_distorcao_infraestrutura_instalacao").submit();
		}
	}
	
	function calculaInstalacao(ifiid)
	{
		var numA = jQuery("[name='num_adequado[" + ifiid + "]']").val();
		var numI = jQuery("[name='num_inadequado[" + ifiid + "]']").val();
		
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
		
		//var total = numA + numI;
		
		//jQuery("[name='total[" + ifiid + "]']").html( mascaraglobal('[.###]',total) );
		
		if(numI && numI != 0){
			jQuery("[name='justificativa[" + ifiid + "]']").attr("readonly","");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("class","normal");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onblur","MouseBlur(this)");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onmouseout","MouseOut(this)");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onfocus","MouseClick(this);this.select()");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onmouseover","MouseOver(this)");
		}else{
			jQuery("[name='justificativa[" + ifiid + "]']").attr("readonly","readonly");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("class","disabled");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onblur","");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onmouseout","");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onfocus","");
			jQuery("[name='justificativa[" + ifiid + "]']").attr("onmouseover","");
			jQuery("[name='justificativa[" + ifiid + "]']").val("");
		}
		
	}
	
	function exibeEspaco(obj)
	{
		if(obj.value == "Sim"){
			salvarRespostaEspaco("true");
			jQuery("#espaco_label").show();
		}else{
			jQuery("#espaco_label").hide();
		}
	}
	
	function gerenciarEspacos(rieid)
	{
		if(rieid){
			rieid = rieid;
		}else{
			rieid = "";
		}
		var html = "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarEspacos&rieid=" + rieid;
		janela(html,500,300,"Espaços");
	}
	
	function carregarEspacos()
	{
		jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarEspacos",
   		async: false,
   		success: function(msg){
   				document.getElementById('espaco_label').innerHTML = msg;
   			}
 		});
	}
	
	function excluirEspaco(rieid) {
		var conf = confirm('Deseja realmente excluir o espaço?');
		if(conf) {
		
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
		   		data: "requisicao=excluirEspaco&rieid="+rieid,
		   		async: false,
		   		success: function(msg){
		   				alert(msg);
						carregarEspacos();
		   			}
		 		});
	 	}
	}
	
	function removeEspacos(obj){
		if(obj.value != "Sim"){
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
		   		data: "requisicao=verificaExisteEspaco",
		   		async: false,
		   		success: function(msg){
		   				if(msg){
							if(confirm("Essa resposta irá remover TODOS OS ESPAÇOS CADASTRADOS. Deseja continuar?")){
								jQuery.ajax({
						   		type: "POST",
						   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
						   		data: "requisicao=excluirTodosEspaco",
						   		async: false,
						   		success: function(msg){
						   				salvarRespostaEspaco("false");
						   				jQuery("#espaco_label").hide();
						   			}
						 		});
							}
						}else{
							salvarRespostaEspaco("false");
						   	jQuery("#espaco_label").hide();
						}
		   			}
		 		});
		}
	}
	
	function salvarRespostaEspaco(resposta)
	{
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=salvarRespostaEspaco&resposta=" + resposta,
	   		async: false,
	   		success: function(msg){
	   				return true;
	   			}
	 		});
	}

	jQuery(document).ready(function(){
		bloquearFormulario('form_distorcao_infraestrutura_instalacao');
	});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0"	align="center">
	<tr>
		<td class="blue center" width="20%" >Orientações</td>
		<td class="blue" >
			<p>Nesta etapa final, vamos conhecer melhor a estrutura física da escola, ou seja, suas instalações internas. Para tanto, precisamos saber quais as dependências que a escola possui, a quantidade e a adequação das mesmas.</p>
			<p>Na tabela abaixo, preencha as colunas relativas à adequação (Adequada/ Inadequada) apenas para as dependências que a escola possui.  Observe que, quando não for indicado um número relativo aos itens "Cozinha", "Salas de aula", "Biblioteca", "Laboratório de informática" e "Sanitários", estes itens serão automaticamente considerados como "Problemas críticos" na Síntese desta dimensão. Além disso, sempre que for preenchdo o campo "Inadequado", será obrigatório descrever na Justificativa o que a escola considera que está em desacordo com as suas necessidades.</p>
			<p>No quadro de perguntas, assinale a resposta correspondente  à situação que mais se aproxima da realidade da escola.</p>
		</td>
	</tr>
</table>
<?php $arrIF = recuperaInstalacoesFisicas(); ?>
<?php $arrDados = recuperaInstalacoesFisicasPorEscola(); ?>
<form name="form_distorcao_infraestrutura_instalacao" id="form_distorcao_infraestrutura_instalacao" method="post" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoInfraestruturaInstalacao" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
		<tr>
			<td class="SubTituloDireita">Condições de uso nas dependências escolares</td>
			<td>
				<?php if($arrIF): ?>
					<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
						<thead>
							<tr>
								<td class="bold center" >Instalações Físicas</td>
								<td class="bold center" >Total</td>
								<td class="bold center" >Adequado(a)</td>
								<td class="bold center" >Inadequado(a)</td>
								<td class="bold center" >Por quê está inadequado(a)?</td>
							</tr>
						</thead>
					<?php $x=1;foreach($arrIF as $ins): ?>
						<?php $cor = $x%2 == 1 ? "#FFFFFF" : "" ?>
						<tr bgcolor="<?php echo $cor ?>">
							<td class="esquerda" id="item_<?php echo  $ins['ifiid'] ?>" ><?php echo $x ?> - <?php echo $ins['ifidesc'] ?></td>
							<td class="center" ><?php echo campo_texto("total[".$ins['ifiid']."]","N","S","","10","20","[.###]","","","","","id='".$ins['ifiid']."'","",($arrDados[$ins['ifiid']]['total'] != "" ? number_format($arrDados[$ins['ifiid']]['total'],"",2,".") : "0") ) ?></td>
							<td class="center" ><?php echo campo_texto("num_adequado[".$ins['ifiid']."]","N","S","","10","20","[.###]","","","","","","calculaInstalacao('{$ins['ifiid']}')",($arrDados[$ins['ifiid']]['adequado'] != "" ? number_format($arrDados[$ins['ifiid']]['adequado'],"",2,".") : "0") ) ?></td>
							<td class="center" ><?php echo campo_texto("num_inadequado[".$ins['ifiid']."]","N","S","","10","20","[.###]","","","","","","calculaInstalacao('{$ins['ifiid']}')",($arrDados[$ins['ifiid']]['inadequado'] != "" ? number_format($arrDados[$ins['ifiid']]['inadequado'],"",2,".") : "0") ) ?></td>
							<td class="esquerda" ><?php echo campo_texto("justificativa[".$ins['ifiid']."]","N",($arrDados[$ins['ifiid']]['justificativa'] ? "S" : "N"),"","60","60","","","","","","","",$arrDados[$ins['ifiid']]['justificativa']) ?></td>
						</tr>
					<?php $x++;endforeach; ?>
					</table>
				<?php endif; ?>
			</td>
		</tr>
		<tr id="tr_perguntas" >
			<td class="SubTituloDireita" width="20%">Perguntas</td>
			<td>
				<?php $sql = "SELECT * FROM pdeinterativo2013.pergunta WHERE prgmodulo = 'I' AND prgsubmodulo = 'I' AND prgstatus = 'A'"; ?>
				<?php quadroPerguntas($sql); ?>
			</td>
		</tr>
		<tr id="tr_espacos" >
				<td class="SubTituloDireita" width="20%">Espaços</td>
				<td>
					<?php $rppresposta = $db->pegaUm("SELECT rppresposta FROM pdeinterativo2013.respostaprojetoprograma WHERE rpptipo = 'E' AND pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']} AND rppstatus = 'A' AND rppmodulo = 'F'"); ?>
					<?php $arrEspacos = existeEspacos(array('sprmodulo'=>'I')) ?>
					Existem espaços públicos ou privados, localizados no entorno da escola, que poderiam ser utilizados para realização de atividades pedagógicas e recreativas? 
					<input type="radio" onclick="exibeEspaco(this)" name="rdo_espaco" <?php echo $rppresposta == "t" ? "checked=checked" : ""; ?> value="Sim" id="rdo_espaco_sim" /> Sim
					<input type="radio" onclick="removeEspacos(this)" name="rdo_espaco" <?php echo $rppresposta == "f" ? "checked=checked" : ""; ?> value="Nao" id="rdo_espaco_nao" /> Não
					<div id="espaco_label" style="display:<?php echo $rppresposta == "t" ? "" : "none"; ?>" >
						<?php carregarEspacos( array("sprmodulo" => "I") ); ?>
					</div>
					
				</td>
			</tr>
		<tr id="tr_navegacao" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_0_orientacoes'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDistorcaoInfraestruturaInstalacao()">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarDistorcaoInfraestruturaInstalacao('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos'" >
			</td>
		</tr>
	</table>
</form>