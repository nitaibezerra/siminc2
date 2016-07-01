<?php 

function listarEntDadosMetas( $entid ){

	global $db;
	
	$sql = "SELECT 
				metid,
				metsemestre, 
				metano, 
				mettxocupacao*100 as mettxocupacao, 
				mettxconclusao*100 as mettxconclusao, 
				metgrupospesquisa, 
				metprojetospesquisa, 
				metservtecnologicos, 
				meteixoscertificacao, 
				metunidadesnit, 
				metescolasapoiadas, 
				metplanejado
			FROM 
				academico.metas
			WHERE
				entidcampus = {$entid}
			ORDER BY
				metano,metsemestre,metplanejado";
	
	$metas = $db->carregar($sql);
	
?>
				<table class="Tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: rgb(220, 220, 220);">
						<td width="10%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">
							<b> Ação </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Semestre </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ano </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Ocupação de vagas (%) </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Concluintes do curso (%) </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Grupos de pesquisa </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Projetos de pesquisa / Inovação </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Serviços tecnológicos / Ação social </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Áreas / Eixos para acreditação / Certificação </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> NIT / Emp. júnior / Inc. empresas implantadas </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Escolas públicas apoiadas </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Tipo da meta </b>
						</td>
					</tr>
<?php 
	$i=0;
	if( is_array($metas) ){
		foreach( $metas as $meta ){
			if( $i % 2 == 1 ){
				$cor = 'rgb(235, 235, 235);';
				$i++;
			}else{
				$cor = 'rgb(255, 255, 255);';
				$i++;
			}
			
			$json = simec_json_encode($meta);
			
			$tipo = $meta['metplanejado'] == 'P' ? 'Planejado' : 'Realizado';
?>
					<tr style="background-color: <?=$cor; ?>">
						<td align="center">
							<img border="0" onclick='carregaMeta(<?=$json;?>);' style="cursor: pointer;" title="Editar" src="/imagens/alterar.gif">
							<img border="0" onclick='excluirMeta(<?=$meta['metid'];?>);' style="cursor: pointer;" title="Editar" src="/imagens/excluir.gif">
						</td>
						<td align="center">
							<?=$meta['metsemestre'] ?>°
						</td>
						<td align="center">
							<?=$meta['metano'] ?>
						</td>
						<td align="center">
							<?=number_format($meta['mettxocupacao'],2,',','.') ?> %
						</td>
						<td align="center">
							<?=number_format($meta['mettxconclusao'],2,',','.') ?> %
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['metgrupospesquisa'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['metprojetospesquisa'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['metservtecnologicos'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['meteixoscertificacao'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['metunidadesnit'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($meta['metescolasapoiadas'],0,',','.') ?>
						</td>
						<td align="center">
							<?=$tipo ?>
						</td>
					</tr>
<?php 		
		}
	}else{
?>
					<tr>
						<td colspan="12" style="color: red">
							Não existem registros cadastrados.
						</td>
					</tr>
<?php 
	}
?>
				</table>
<?php 

}

function salvaEntDadosMetas ( $dados ){
	
	global $db;
	
	//Tratamento de valores do Padrão brasileiro para padrão Americano(do Banco)
    $dados['metgrupospesquisa']    = str_replace(".","",$dados['metgrupospesquisa']);
	$dados['metprojetospesquisa']  = str_replace(".","",$dados['metprojetospesquisa']);
	$dados['metservtecnologicos']  = str_replace(".","",$dados['metservtecnologicos']);
	$dados['meteixoscertificacao'] = str_replace(".","",$dados['meteixoscertificacao']);
	$dados['metunidadesnit']	   = str_replace(".","",$dados['metunidadesnit']);
	$dados['metescolasapoiadas']   = str_replace(".","",$dados['metescolasapoiadas']);
	$dados['mettxocupacao']	   	   = str_replace(",",".",$dados['mettxocupacao']);
	$dados['mettxconclusao']  	   = str_replace(",",".",$dados['mettxconclusao']);
		
	//Tratamento para campos vazios
    $dados['mettxocupacao']  	   = $dados['mettxocupacao'] 	    ? $dados['mettxocupacao']/100	 : 'null';
    $dados['mettxconclusao']  	   = $dados['mettxconclusao'] 	    ? $dados['mettxconclusao']/100   : 'null';
    $dados['metgrupospesquisa']    = $dados['metgrupospesquisa']    ? $dados['metgrupospesquisa']    : 'null';
	$dados['metprojetospesquisa']  = $dados['metprojetospesquisa']  ? $dados['metprojetospesquisa']  : 'null';
	$dados['metservtecnologicos']  = $dados['metservtecnologicos']  ? $dados['metservtecnologicos']  : 'null';
	$dados['meteixoscertificacao'] = $dados['meteixoscertificacao'] ? $dados['meteixoscertificacao'] : 'null';
	$dados['metunidadesnit']	   = $dados['metunidadesnit']	    ? $dados['metunidadesnit'] 		 : 'null';
	$dados['metescolasapoiadas']   = $dados['metescolasapoiadas']   ? $dados['metescolasapoiadas']   : 'null';
	$dados['metplanejado']		   = $dados['metplanejado']		    ? $dados['metplanejado'] 		 : 'P';
	
	$sql = "SELECT 
				metid, 
				metsemestre, 
				metano, 
				mettxocupacao, 
				mettxconclusao, 
				metgrupospesquisa, 
				metprojetospesquisa, 
				metservtecnologicos, 
				meteixoscertificacao, 
				metunidadesnit, 
				metescolasapoiadas, 
				metplanejado
			FROM 
				academico.metas
			WHERE
				entidcampus    = {$dados['entid']} AND
				metano         = {$dados['metano']} AND
				metsemestre    = {$dados['metsemestre']} AND
				metplanejado   = '{$dados['metplanejado']}'";
	
	$metid = $db->pegaUm($sql);
	
	//Se possuir meta, atualiza, se não, insere meta
	if( $metid && $dados['metid']){
		
		$sql = "UPDATE academico.metas
			    SET 
			    	metsemestre 		 = {$dados['metsemestre']}, 
			    	metano 				 = {$dados['metano']}, 
			    	mettxocupacao 		 = {$dados['mettxocupacao']}, 
			        mettxconclusao 		 = {$dados['mettxconclusao']}, 
			        metgrupospesquisa 	 = {$dados['metgrupospesquisa']}, 
			        metprojetospesquisa  = {$dados['metprojetospesquisa']}, 
			        metservtecnologicos  = {$dados['metservtecnologicos']}, 
			        meteixoscertificacao = {$dados['meteixoscertificacao']}, 
			        metunidadesnit 		 = {$dados['metunidadesnit']}, 
			        metescolasapoiadas 	 = {$dados['metescolasapoiadas']}, 
			        metplanejado 		 = '{$dados['metplanejado']}'
			 	WHERE 
			 		metid = {$metid}";
		
	}else{
		if( $metid ){
			
			$tipoMeta = $dados['metplanejado'] == 'P' ? 'Planejado' : 'Realizado';
			
			echo "  <script>
						alert('A meta para o {$dados['metsemestre']}° Semestre no ano {$dados['metano']} no tipo \'{$tipoMeta}\' já existe.');
					</script>";
			$falha = true;
			
		}else{
			$sql = "INSERT INTO academico.metas(
				            entidcampus, 
				            metsemestre, 
				            metano, 
				            mettxocupacao, 
				            mettxconclusao, 
				            metgrupospesquisa, 
				            metprojetospesquisa, 
				            metservtecnologicos, 
				            meteixoscertificacao, 
				            metunidadesnit, 
				            metescolasapoiadas, 
				            metplanejado)
				    VALUES ({$dados['entid']}, 
				    		{$dados['metsemestre']}, 
				    		{$dados['metano']}, 
				    		{$dados['mettxocupacao']}, 
				    		{$dados['mettxconclusao']},
				    		{$dados['metgrupospesquisa']}, 
				            {$dados['metprojetospesquisa']}, 
				            {$dados['metservtecnologicos']}, 
				            {$dados['meteixoscertificacao']}, 
				            {$dados['metunidadesnit']}, 
				            {$dados['metescolasapoiadas']}, 
				            '{$dados['metplanejado']}')";
		}
		
	}
		
	//Se não possuir erro comita.
	if( !$falha ){
		if( $db->executar($sql) ){
			$db->commit();
			echo "<script>alert('Dados salvos com sucesso!');</script>";
		}
	}

}

if( $_REQUEST['req'] == 'excluir' ){
	
	$sql = "DELETE FROM
				academico.metas
			WHERE
				metid = {$_REQUEST['metid']}";
	
	$db->executar($sql);
	$db->commit();
	
	echo listarEntDadosMetas($_REQUEST['entid']);
	die();	
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 

$existecampus = academico_existecampus( $entidcampus );

if( !$existecampus ){
	echo "<script>
			alert('O Campus informado não existe!');
			history.back(-1);
		  </script>";
	die;
}

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   //PERFIL_IFESCADCURSOS, 
					   //PERFIL_IFESCADASTRO, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

$bloqueado = $permissoes['gravar'] ? false :  true;

// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus, $tipoEntidade);

if($entidcampus){
	$_SESSION['academico']['entidadenivel'] = 'campus';
}

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Cadastro de Metas", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$entidcampus) {
	echo "<script>alert('Campus não escolhido.');window.location='?modulo=inicio&acao=C';</script>";
	exit;
}

$entidade = new Entidades();

$entidade->carregarPorEntid($entidcampus);

// Gravar informações
if( $_REQUEST['req'] == 'gravar' ){
	salvaEntDadosMetas( $_REQUEST );
	unset($_REQUEST['req']);
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>

<link rel="stylesheet" href="/includes/entidadesn.css"                   type="text/css" media="screen" />

<form name="frmMetas" id="frmMetas" method="post" action="" onsubmit="">

	<input type="hidden" id="entid" name="entid" value="<?=$entidcampus; ?>">
	<input type="hidden" id="metid" name="metid" value="">
	<input type="hidden" id="req"   name="req"   value="0">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
		<tr>
			<td></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Ano:
			</td>
			<td>
				<select id="metano" name="metano" >
					<option value=""> --- </option>
					<option value="2008">2008</option>
					<option value="2009">2009</option>
					<option value="2010">2010</option>
					<option value="2011">2011</option>
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="250px"> Semestre:
			</td>
			<td>	
				<input type="radio" name="metsemestre" id="semestre1" value="1" /> 1° Semestre
				<input type="radio" name="metsemestre" id="semestre2" value="2" /> 2° Semestre
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Percentual de ocupação de vagas:
			</td>
			<td>
				<?= campo_texto( 'mettxocupacao', 'N', 'S', '', 6, 6, '###,##', '','','','','id="mettxocupacao"','this.value=verificaPercentual(this.value);','','this.value=verificaPercentual(this.value);' ); ?>
				(%)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Percentual de concluintes por ingressantes no curso:
			</td>
			<td>
				<?= campo_texto( 'mettxconclusao', 'N', 'S', '', 6, 6, '###,##', '','','','','id="mettxconclusao"','this.value=verificaPercentual(this.value);','','this.value=verificaPercentual(this.value);'  ); ?>
				(%)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de grupos de pesquisa:
			</td>
			<td>
				<?= campo_texto( 'metgrupospesquisa', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="metgrupospesquisa"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de projetos de pesquisa / inovação:
			</td>
			<td>
				<?= campo_texto( 'metprojetospesquisa', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="metprojetospesquisa"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de serviços tecnológicos / ação Social:
			</td>
			<td>
				<?= campo_texto( 'metservtecnologicos', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="metservtecnologicos"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de áreas / eixos para acreditação / certificação:
			</td>
			<td>
				<?= campo_texto( 'meteixoscertificacao', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="meteixoscertificacao"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Unidades de NIT / Emp. júnior / Inc. empresas implantadas:
			</td>
			<td>
				<?= campo_texto( 'metunidadesnit', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="metunidadesnit"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Escolas públicas apoiadas:
			</td>
			<td>
				<?= campo_texto( 'metescolasapoiadas', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="metescolasapoiadas"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Tipo da meta:
			</td>
			<td>
				<input type="radio" name="metplanejado" id="metplanejadoP" value="P" /> Planejado
				<input type="radio" name="metplanejado" id="metplanejadoR" value="R" /> Realizado
			</td>
		</tr>
		<tr>
			<td> 
			</td>
			<td>
				<?php if( $permissoes['gravar'] ){ ?>
					<input type="button" value="Gravar" onclick="validaForm();"></input>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td></td>
			<td id="tdMeta">
<?php 
	echo listarEntDadosMetas($entidcampus);
?>
			</td>
		</tr>
	</table>
</form>
<script>

function carregaMeta( dados ){

	var metid   = $('metid');
	metid.value = dados.metid; 
	
	var metsemestre1 = $('semestre1');
	var metsemestre2 = $('semestre2');
	if( dados.metsemestre == '1' ){
		metsemestre1.checked = true;
	}else{
		metsemestre2.checked = true;
	}
	
	var metano   = $('metano');
	metano.value = dados.metano;
	
	var mettxocupacao   = $('mettxocupacao');
	
	mettxocupacao.value = mascaraglobal('###,##',Number(dados.mettxocupacao).toFixed(2));
	
	var mettxconclusao   = $('mettxconclusao');
	mettxconclusao.value = mascaraglobal('###,##',Number(dados.mettxconclusao).toFixed(2));
	
	var metgrupospesquisa   = $('metgrupospesquisa');
	metgrupospesquisa.value = dados.metgrupospesquisa;
	
	var metprojetospesquisa  = $('metprojetospesquisa');
	metprojetospesquisa.value = dados.metprojetospesquisa;
	
	var metservtecnologicos   = $('metservtecnologicos');
	metservtecnologicos.value = dados.metservtecnologicos;
	
	var meteixoscertificacao   = $('meteixoscertificacao');
	meteixoscertificacao.value = dados.meteixoscertificacao;
	
	var metunidadesnit 		   = $('metunidadesnit');
	metunidadesnit.value = dados.metunidadesnit;
	
	var metescolasapoiadas   = $('metescolasapoiadas');
	metescolasapoiadas.value = dados.metescolasapoiadas;
	
	var metplanejadoP = $('metplanejadoP');
	var metplanejadoR = $('metplanejadoR');
	if( dados.metplanejado == 'P' ){
		metplanejadoP.checked = true;
	}else{
		metplanejadoR.checked = true;
	}

}

function excluirMeta( metid ){

	if(confirm('Realmente deseja excluir este registro?')){
		var td    = $('tdMeta');
		var entid = $('entid').value;
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: '&req=excluir&metid=' + metid + '&entid=' + entid,
			onComplete: function(res){
				alert('Registro Excluido.');
				td.innerHTML = res.responseText;
			}
		});
	}

}

function verificaPercentual ( valor ){

	valor = mascaraglobal('###.##',valor);

	if( valor > 100 ){
		valor = 100;
	}else{
		valor = mascaraglobal('###,##',valor);
	}

	return valor;
}

function validaForm(){

	var ano  = document.getElementById('metano');
	
	var semestre1 = document.getElementById('semestre1');
	var semestre2 = document.getElementById('semestre2');
	
	var form = document.getElementById('frmMetas');
	var req  = document.getElementById('req');

	if( ano.value == '---' || ano.value == '' ){
		alert('Campo Obrigatório');
		ano.focus();
		return false;
	}

	if( !semestre1.checked && !semestre2.checked ) {
		alert('Campo obrigatório');
		semetre1.focus();
		return false;
	}

	req.value = 'gravar';
	form.submit();
}

</script>


