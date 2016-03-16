<? 
$_REQUEST['fpbid'] = preg_replace("/[^0-9]/", "", $_REQUEST['fpbid']);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('uncid' => $_SESSION['sismedio']['professoralfabetizador']['uncid'], 'fpbid' => $_REQUEST['fpbid'], 'pflcod_avaliador' => PFL_PROFESSORALFABETIZADOR));
	?>
	</td>
</tr>
</table>
<?
if($_REQUEST['fpbid']) :

	if(!$_SESSION['sismedio']['professoralfabetizador']['uncid'] || !$_SESSION['sismedio']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Problemas para acessar essa tela, por favor faça o LOGOUT e acesse novamente.","location"=>"sismedio.php?modulo=inicio&acao=C");
		alertlocation($al);		
	}

	
	$sql = "SELECT rfuparcela FROM sismedio.folhapagamentouniversidade WHERE fpbid='".$_REQUEST['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sismedio']['professoralfabetizador']['uncid']."'";
	$rfuparcela = $db->pegaUm($sql);
	
	?>
	<script>
	
	function oeProposAtividadeCadernoFormacao(obj) {
		
		if(obj.value=='2') {
			var conf = confirm('Confirma que o Orientador de Estudo NÃO propôs uma ou mais atividades previstas nos Cadernos? Caso tenha alguma atividade cadastrada, essas serão removidas');

			if(!conf) {
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").attr('checked', true);
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").click();
				return false;
			}
		}

		if(obj.value=='3') {
			var conf = confirm('Confirma que NÃO foi utilizado o(s) Caderno(s) de Formação fornecidos digitalmente pelo MEC? Caso tenha alguma atividade cadastrada, essas serão removidas');

			if(!conf) {
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").attr('checked', true);
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").click();
				return false;
			}
		}

		if(obj.value=='4') {
			var conf = confirm('Confirma que NÃO que não realizou nenhuma atividade nesse período? Caso tenha alguma atividade cadastrada, essas serão removidas');

			if(!conf) {
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").attr('checked', true);
				jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']").click();
				return false;
			}
		}

		ajaxatualizar('requisicao=gravarOEProposAtividadeCadernoFormacao&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&fpbid=<?=$_REQUEST['fpbid'] ?>&caroeproposatividadecadernoformacao='+obj.value,'');

		if(obj.value=='1') {
			
			jQuery('#tr_atividadescaderno').css('display','');
			
		} else {
			
			jQuery('#tr_atividadescaderno').css('display','none');
			
		}
	}

	function inserirAtividade() {
		
		jQuery("#modalInserirAtividades").dialog({
            draggable:true,
            resizable:true,
            width: 800,
            height: 600,
            modal: true,
         	close: function(){} 
        });
	
	}

	function carregarAtividadesCaderno(cadidpai) {
		
		if(cadidpai=='') {
			jQuery('#td_atividadecaderno').html('--- SELECIONE O CADERNO ---');
		} else {
			ajaxatualizar('requisicao=carregarAtividadesCaderno&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&cadidpai='+cadidpai,'td_atividadecaderno');
		}

	}

	function gravarAtividade() {

		if(jQuery('#cadidpai').val()=='') {
			alert('Selecione um caderno');
			return false;
		}


		if(jQuery('#cadid').length==0) {
			alert('Selecione um caderno');
			return false;
		}

		if(document.getElementById("cadid").value=='') {
			alert('Selecione uma atividade');
			return false;
		}
		

		ajaxatualizar('requisicao=inserirAtividadeCaderno&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&fpbid=<?=$_REQUEST['fpbid'] ?>&cadid='+document.getElementById("cadid").value,'');
		ajaxatualizar('requisicao=listaAtividadesCadernoFormacao&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&fpbid=<?=$_REQUEST['fpbid'] ?>','dv_atividadescaderno');

		jQuery("#modalInserirAtividades").dialog('close');
		
	}

	function atualizarAtividadeCaderno(carid, campo, valor) {

		if(campo=='carformarealizacao') {
			
			if(valor=='I') {
				jQuery('#dv_carformarealizacao_'+carid).css('display','');
			} else {
				jQuery('#dv_carformarealizacao_'+carid).css('display','none');
			}

		}
		
		ajaxatualizar('requisicao=atualizarAtividadeCaderno&carid='+carid+'&campo='+campo+'&valor='+valor,'');
	}

	function atualizarAtividadeCadernoTema(obj) {

		var param='';

		if(obj.checked) {
			param = '&tipo=inserir&catid='+obj.value;
		} else {
			param = '&tipo=remover&catid='+obj.value;
		}
		
		ajaxatualizar('requisicao=gravarAtividadeCadernoTema&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&fpbid=<?=$_REQUEST['fpbid'] ?>'+param,'');
		
	}

	function excluirAtividadeCadernoFormacao(carid) {
		var conf = confirm('Deseja realmente excluir essa atividade?');

		if(conf) {
			ajaxatualizar('requisicao=excluirAtividadeCaderno&carid='+carid,'');
			ajaxatualizar('requisicao=listaAtividadesCadernoFormacao&iusd=<?=$_SESSION['sismedio']['professoralfabetizador']['iusd'] ?>&fpbid=<?=$_REQUEST['fpbid'] ?>','dv_atividadescaderno');
		}
	}

	function validarListaAtividadesCadernoFormacao() {

		var caroeproposatividadecadernoformacao    = jQuery("[name^='caroeproposatividadecadernoformacao']:enabled:checked").length;
		if(caroeproposatividadecadernoformacao==0) {
			alert('Marque se o seu Orientador de Estudos propôs que realizasse atividade(s) do(s) Caderno(s) de Formação');
			return false;
		}

		var caroeproposatividadecadernoformacao_sim    = jQuery("[name^='caroeproposatividadecadernoformacao'][value^='1']:enabled:checked").length;

		if(caroeproposatividadecadernoformacao_sim==1) {
			var total_atividades = (jQuery("[name^='carformarealizacao']:enabled").length/2);

			var carformarealizacao_marcados    = jQuery("[name^='carformarealizacao']:enabled:checked").length;

			if(carformarealizacao_marcados!=total_atividades) {
				alert('Marque a Forma de realização');
				return false;
			}

			var caravaliacaooe_marcados    = jQuery("[name^='caravaliacaooe']:enabled:checked").length;

			if(caravaliacaooe_marcados!=total_atividades) {
				alert('Marque se a Atividade realizada foi avaliada pelo Orientador de Estudo');
				return false;
			}

			var carconteudocadernoformacao_marcados    = jQuery("[name^='carconteudocadernoformacao']:enabled:checked").length;

			if(carconteudocadernoformacao_marcados!=total_atividades) {
				alert('Marque a Relação com o conteúdo do Caderno de Formação');
				return false;
			}
			
			var carclarezainstrucoesatividades_marcados    = jQuery("[name^='carclarezainstrucoesatividades']:enabled:checked").length;

			if(carclarezainstrucoesatividades_marcados!=total_atividades) {
				alert('Marque a Clareza das instruções para realização da atividade');
				return false;
			}
			
			var carclarezaobjetivosatividades_marcados    = jQuery("[name^='carclarezaobjetivosatividades']:enabled:checked").length;

			if(carclarezaobjetivosatividades_marcados!=total_atividades) {
				alert('Marque a Clareza do objetivo da realização da atividade');
				return false;
			}

			var carvinculacaocontexto_marcados    = jQuery("[name^='carvinculacaocontexto']:enabled:checked").length;

			if(carvinculacaocontexto_marcados!=total_atividades) {
				alert('Marque a Vinculação ao contexto de sala de aula/cotidiano escolar');
				return false;
			}
			
			var cararticulacaoteoriapratica_marcados    = jQuery("[name^='cararticulacaoteoriapratica']:enabled:checked").length;

			if(cararticulacaoteoriapratica_marcados!=total_atividades) {
				alert('Marque a Articulação entre teoria e prática');
				return false;
			}
			
		}

		alert('Dados gravados com sucesso');

		
	}
	
	</script>
	
	<div id="modalInserirAtividades" style="display:none;">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	
	<tr>
		<td class="SubTituloEsquerda">Cadernos:</td>
		<td>
		<?
		$sql = "SELECT cadid as codigo, caddsc as descricao FROM sismedio.cadernoatividades WHERE cadidpai IS NULL ORDER BY cadid";
		$db->monta_combo('cadidpai', $sql, 'S', 'Selecione', 'carregarAtividadesCaderno', '', '', '', 'N', 'cadidpai','');
		?>
		</td>
	</tr>

	<tr>
		<td class="SubTituloEsquerda">Atividades:</td>
		<td id="td_atividadecaderno">--- SELECIONE O CADERNO ---</td>
	</tr>
	
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="gravaratividade" value="Gravar Atividade" onclick="gravarAtividade();"></td>
	</tr>
	
	
	</table>
	
	
	</div>
	
	
	<? $caroeproposatividadecadernoformacao = $db->pegaUm("SELECT caroeproposatividadecadernoformacao FROM sismedio.cadernoatividadesrespostas WHERE fpbid='".$_REQUEST['fpbid']."' AND iusd='".$_SESSION['sismedio']['professoralfabetizador']['iusd']."' AND caroeproposatividadecadernoformacao IS NOT NULL") ?>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
		<p>Propõe-se, como parte das atividades individuais do processo formativo na escola, que pelo menos uma das atividades propostas no material de formação fornecido digitalmente pelo MEC seja realizada por cada professor cursista.</p>
		<p>Nesse período, o seu Orientador de Estudos propôs que realizasse atividade(s) do(s) Caderno(s) de Formação?</p>
		<p>
		<input type="radio" name="caroeproposatividadecadernoformacao" value="1" onclick="oeProposAtividadeCadernoFormacao(this);" <?=(($caroeproposatividadecadernoformacao=='1')?'checked':'') ?> > Sim, o Orientador de Estudo propôs uma ou mais atividades previstas nos Cadernos.<br>
		<input type="radio" name="caroeproposatividadecadernoformacao" value="2" onclick="oeProposAtividadeCadernoFormacao(this);" <?=(($caroeproposatividadecadernoformacao=='2')?'checked':'') ?> > Não, o Orientador de Estudo não propôs uma ou mais atividades previstas nos Cadernos.<br>
		<input type="radio" name="caroeproposatividadecadernoformacao" value="3" onclick="oeProposAtividadeCadernoFormacao(this);" <?=(($caroeproposatividadecadernoformacao=='3')?'checked':'') ?> > Não utilizamos o(s) Caderno(s) de Formação fornecidos digitalmente pelo MEC.<br>
		<input type="radio" name="caroeproposatividadecadernoformacao" value="4" onclick="oeProposAtividadeCadernoFormacao(this);" <?=(($caroeproposatividadecadernoformacao=='4')?'checked':'') ?> > Não realizei as atividades nesse período.
		</p>
		</td>
	</tr>
	<tr id="tr_atividadescaderno" <?=(($caroeproposatividadecadernoformacao=='1')?'':'style="display: none;"') ?> >
		<td>
		<p><input type="button" name="inseriratividade" value="Inserir Atividade" onclick="inserirAtividade();"></p>
		
		<div id="dv_atividadescaderno">
		
		<? 
		
		listaAtividadesCadernoFormacao(array('iusd'=>$_SESSION['sismedio']['professoralfabetizador']['iusd'],'fpbid'=>$_REQUEST['fpbid']));
		
		?>
		
		</div>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro">
		
		<input type="button" name="gravar" value="Gravar" onclick="validarListaAtividadesCadernoFormacao();">
		
		</td>
	</tr>
	</table>
	<?

endif;

?>