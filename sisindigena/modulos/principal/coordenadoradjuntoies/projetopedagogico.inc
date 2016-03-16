<? 

verificarTermoCompromisso(array("iusd"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd'],"sis"=>'coordenadoradjuntoies',"pflcod"=>PFL_COORDENADORADJUNTOIES)); 

$estado = wf_pegarEstadoAtual( $_SESSION['sisindigena']['coordenadoradjuntoies']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

?>
<script>
function selecionarEixoPedagogico(obj) {
	if(obj.checked) {
		jQuery('#tr_eprobs_'+obj.value).css('display','');
	} else {
		jQuery('#tr_eprobs_'+obj.value).css('display','none');
		jQuery('#eprobs_'+obj.value).val('');
	}
}

function inserirAldeia() {

	if(jQuery('#lanaldeiadsc').val()=='') {
		alert('Digite o nome da aldeia');
		return false;
	}
	
	ajaxatualizar('requisicao=inserirAldeia&picid=<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>&lanaldeiadsc='+jQuery('#lanaldeiadsc').val(),'');
	ajaxatualizar('requisicao=quadroAbrangenciaAcao&grid=aldeia&picid=<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>','td_aldeia');
	
}

function excluirRegistro(lanid,grid) {
	var conf = confirm('Deseja realmente remover este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=removerAbrangenciaAcao&lanid='+lanid,'');
		ajaxatualizar('requisicao=quadroAbrangenciaAcao&grid='+grid+'&picid=<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>','td_'+grid);
	}

}

function inserirEscola(escoladescricao) {
	ajaxatualizar('requisicao=inserirEscola&picid=<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>&lanescoladsc='+escoladescricao,'');
	ajaxatualizar('requisicao=quadroAbrangenciaAcao&grid=escola_atendida&picid=<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>','td_escola_atendida');

}


function carregarEscolasPorMunicipio(muncod) {
	if(muncod) {
		ajaxatualizar('requisicao=carregarEscolasPorMunicipio&muncod='+muncod,'td_escola');
	} else {
		document.getElementById('td_escola').innerHTML = "Selecione Município";
	}
}

function carregarMunicipiosPorUFProjetoPedagogico(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&onclick=carregarEscolasPorMunicipio&id=muncod_esc&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

function salvarProjetoPedagogico(goto) {
	divCarregando();
	jQuery('#goto').val(goto);
	jQuery('#formulario').submit();
}

function exibeBusca(obj) {
	if(obj.checked) {
		if(obj.value=='1') {
			jQuery('#tr_neduca').css('display','none');
			jQuery('#tr_educa').css('display','');
		}
		
		if(obj.value=='2') {
			jQuery('#tr_educa').css('display','none');
			jQuery('#tr_neduca').css('display','');
		}
	
	}
}
</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gravarProjetoPedagogico">
<input type="hidden" name="picid" value="<?=$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'] ?>">
<input type="hidden" name="goto" id="goto" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Projeto Pedagógico</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Abrangência da Ação Saberes Indígenas</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloEsquerda">Território Etnoeducacionais</td>
		</tr>
		<tr>
			<td><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'laatipo'=>'etnoeducacionais','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Povos</td>
		</tr>
		<tr>
			<td><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'laatipo'=>'povos','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Aldeias</td>
		</tr>
		<tr>
			<td id="td_aldeia"><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'grid'=>'aldeia','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Línguas Faladas</td>
		</tr>
		<tr>
			<td><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'laatipo'=>'lingua','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Mapas</td>
		</tr>
		<tr>
			<td id="td_mapa"><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'grid'=>'mapa','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Escolas Atendidas</td>
		</tr>
		<tr>
			<td id="td_escola_atendida"><? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'grid'=>'escola_atendida','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?></td>
		</tr>


		</table>
	</tr>	
	<? 
	
	$nucleouniversidade = $db->pegaLinha("SELECT picmetodologiaaplicada, 
												 picmetodologiaavaliacao, 
												 picmetodologiaacompanhamento,
												 picsituacaosociolinguistica 
										  FROM sisindigena.nucleouniversidade 
										  WHERE picid='".$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']."'");

	extract($nucleouniversidade);
	?>

	
	<tr>
		<td class="SubTituloDireita" width="20%">Diagnóstico da Situação Sociolinguística</td>
		<td>
		<? quadroAbrangenciaAcao(array('visrelatorio'=>$consulta,'laatipo'=>'sociolinguistica','picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])) ?>
		<p>Preencha o campo com uma análise da situação sociolinguística</p>
		<? echo campo_textarea( 'picsituacaosociolinguistica', 'S', (($consulta)?'N':'S'), '', '70', '4', '1000'); ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Eixos da Formação Continuada Desenvolvidos</td>
		<td>
		<? 
 		$sql = "SELECT * FROM sisindigena.eixospedagogicos WHERE expstatus='A'";
 		$eixospedagogicos = $db->carregar($sql);
		?>
		
		<? if($eixospedagogicos[0]) : ?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<? foreach($eixospedagogicos as $ep) : ?>
		<?
		$eixospedagogicosuniversidade = $db->pegaLinha("SELECT epuid, eprobs FROM sisindigena.eixospedagogicosnucleo WHERE expid='".$ep['expid']."' AND picid='".$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']."'");
		$obs = 'eprobs_'.$ep['expid'];
		$$obs = $eixospedagogicosuniversidade['eprobs'];
		?>
		<tr>
			<td width="5"><input type="checkbox" <?=(($consulta)?"disabled":"") ?> name="expid[]" value="<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"checked":"") ?> onclick="selecionarEixoPedagogico(this);"></td>
			<td><?=$ep['expdsc'] ?></td>
		</tr>
		<tr id="tr_eprobs_<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"":"style=\"display:none;\"") ?> >
			<td colspan="2">
			<p>Preencha o campo com os Objetivos Gerais e Específicos ao Eixo</p>
			<? echo campo_textarea( 'eprobs_'.$ep['expid'], 'S', (($consulta)?'N':'S'), '', '70', '4', '1000'); ?>
			</td>
		</tr>
		<? endforeach; ?>
		</table>
		<? else : ?>
		Não existem eixos pedagógicos
		<? endif; ?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" width="20%">Metodologia</td>
		<td>
		Preencha o campo com a descrição da metodologia aplicada
		<? echo campo_textarea( 'picmetodologiaaplicada', 'S', (($consulta)?'N':'S'), '', '70', '4', '1000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Processo de Avaliação</td>
		<td>
		<b>Avaliação do Curso</b><br>
		Preencha o campo com as metodologias que serão utilizadas na avaliação do curso
		<? echo campo_textarea( 'picmetodologiaavaliacao', 'S', (($consulta)?'N':'S'), '', '70', '4', '1000'); ?>
		<br>
		<b>Avaliação do Cursista</b><br>
		Preencha o campo com as metodologias que serão utilizadas no acompanhamento a na avaliação do cursista
		<? echo campo_textarea( 'picmetodologiaacompanhamento', 'S', (($consulta)?'N':'S'), '', '70', '4', '1000'); ?>
		</td>
	</tr>


	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=dados';">
			<? if(!$consulta) : ?> 
				<input type="button" name="salvar" value="Salvar" onclick="salvarProjetoPedagogico('sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=projetopedagogico');"> 
				<input type="button" value="Salvar e Continuar" onclick="salvarProjetoPedagogico('sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=definirequipe');"> 
			<? endif; ?>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=definirequipe';">
		</td>
	</tr>
</table>
</form>