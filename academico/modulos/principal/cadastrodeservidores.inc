<?php 

function listaServidor ( $entid ){
	
	global $db;
	
	$sql = "SELECT 
				serid,  
				serano, 
				sersemestre, 
				serqtdde, 
				serplanejado,
				serqtde20, 
				serqtde40 
			FROM 
				academico.servidores
			WHERE
				entidcampus = {$entid}
			ORDER BY
				serano,sersemestre,serplanejado";
	
	$servidores = $db->carregar($sql);
	
?>
				<table class="Tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: rgb(220, 220, 220);">
						<td width="10%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ação </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Semestre </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ano </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Novos Docentes DE </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Tipo (Planejado ou Realizado) </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Novos Docentes 20 H </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Novos Docentes 40 H </b>
						</td>
					</tr>
<?php 
	$i=0;
	if( is_array($servidores) ){
		foreach( $servidores as $servidor ){
			if( $i % 2 == 1 ){
				$cor = 'rgb(235, 235, 235);';
				$i++;
			}else{
				$cor = 'rgb(255, 255, 255);';
				$i++;
			}
			
			$json = simec_json_encode($servidor);
			
			$tipo = $servidor['serplanejado'] == 'P' ? 'Planejado' : 'Realizado';
?>
					<tr style="background-color: <?=$cor; ?>">
						<td align="center">
							<img border="0" onclick='carregaServidor(<?=$json;?>);' style="cursor: pointer;" title="Editar" src="/imagens/alterar.gif">
							<img border="0" onclick='excluirServidor(<?=$servidor['serid'];?>);' style="cursor: pointer;" title="Editar" src="/imagens/excluir.gif">
						</td>
						<td align="center">
							<?=$servidor['sersemestre'] ?>°
						</td>
						<td align="center">
							<?=$servidor['serano'] ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($servidor['serqtdde'],0,',','.') ?>
						</td>
						<td align="center">
							<?=$tipo ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($servidor['serqtde20'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($servidor['serqtde40'],0,',','.') ?>
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

function salvaEntDadosServidor ( $dados ){
	
	global $db;
	
	//Tratamento de valores do Padrão brasileiro para padrão Americano(do Banco)
    $dados['serqtdde']    = str_replace(".","",$dados['serqtdde']);
	$dados['serqtde20']   = str_replace(".","",$dados['serqtde20']);
	$dados['serqtde40']   = str_replace(".","",$dados['serqtde40']);
	
    //Tratamento para campos vazios
    $dados['serqtdde']	   = $dados['serqtdde']	    ? $dados['serqtdde'] 	 : 'null';
    $dados['serplanejado'] = $dados['serplanejado'] ? $dados['serplanejado'] : 'P';
    $dados['serqtde20']    = $dados['serqtde20']    ? $dados['serqtde20']    : 'null';
    $dados['serqtde40']    = $dados['serqtde40']    ? $dados['serqtde40']    : 'null';
	
    //testa servidor existente
    $sql = "SELECT 
				serid 
			FROM 
				academico.servidores
			WHERE
				entidcampus  = {$dados['entid']} AND
				serano       = {$dados['serano']} AND
				sersemestre  = {$dados['sersemestre']} AND
				serplanejado = '{$dados['serplanejado']}'";
	
	$serid = $db->pegaUm($sql);
    
    //Se possuir servidor, atualiza, se não, insere servidor
	if( $serid && !empty($dados['serid']) ){
		
		$sql = "UPDATE 
					academico.servidores
				SET 
					serano 		 = {$dados['serano']}, 
					sersemestre  = {$dados['sersemestre']}, 
					serqtdde 	 = {$dados['serqtdde']}, 
			        serplanejado = '{$dados['serplanejado']}',
			        serqtde20 	 = {$dados['serqtde20']}, 
			        serqtde40 	 = {$dados['serqtde40']}
				WHERE 
					serid =  {$dados['serid']}";
		
	}else{
		
		if( $serid ){
			
			$textoServTipo = $dados['serplanejado'] == 'P' ? 'Planejado' : 'Realizado';
			
			echo "  <script>
						alert('O cadastro de servidores do {$dados['sersemestre']}° semestre de {$dados['serano']} do tipo \'{$textoServTipo}\' já existe.\'');
					</script>";
			
			$erro = true;
			
		}else{
			
			$sql = "INSERT INTO academico.servidores(
				            entidcampus, 
				            serano, 
				            sersemestre, 
				            serqtdde, 
				            serplanejado,
				            serqtde20, 
				            serqtde40) 
				    VALUES ({$dados['entid']}, 
				    		{$dados['serano']}, 
				    		{$dados['sersemestre']}, 
				    		{$dados['serqtdde']}, 
				    		'{$dados['serplanejado']}',
				    		{$dados['serqtde20']}, 
				            {$dados['serqtde40']})";
				            
		}
		
	}
	
	//Se não possuir erro comita.
	if( !$erro ){
		if($db->executar($sql)){
			$db->commit();
			echo "<script>alert('Dados salvos com sucesso!');</script>";
		}
	}

}

if( $_REQUEST['req'] == 'excluir' ){
	
	$sql = "DELETE FROM
				academico.servidores
			WHERE
				serid = {$_REQUEST['serid']}";
	
	$db->executar($sql);
	$db->commit();
	
	echo listaServidor($_REQUEST['entid']);
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

#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Cadastro de Servidores", "");

// cria o cabeçalho padrão do módulo
#$autoriazacaoconcursos = new autoriazacaoconcursos();
#echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$entidcampus) {
	echo "<script>alert('Campus não escolhido.');window.location='?modulo=inicio&acao=C';</script>";
	exit;
}

$entidade = new Entidades();

$entidade->carregarPorEntid($entidcampus);

// Gravar informações
if( $_REQUEST['req'] == 'gravar' ){
	salvaEntDadosServidor( $_REQUEST );
	unset($_REQUEST['req']);
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>

<link rel="stylesheet" href="/includes/entidadesn.css" type="text/css" media="screen" />
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    // cria o cabeçalho padrão do módulo
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);
    ?>
<form name="frmMetas" id="frmMetas" method="post" action="" onsubmit="">

	<input type="hidden" id="entid" name="entid" value="<?=$entidcampus; ?>">
	<input type="hidden" id="serid" name="serid" value="">
	<input type="hidden" id="req"   name="req"   value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
		<tr>
			<td></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Ano:
			</td>
			<td>
				<select id="serano" name="serano" >
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
				<input type="radio" name="sersemestre" id="sersemestre1" value="1" checked="checked" /> 1° Semestre
				<input type="radio" name="sersemestre" id="sersemestre2" value="2" /> 2° Semestre
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Novos Docentes DE:
			</td>
			<td>
				<?= campo_texto( 'serqtdde', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="serqtdde"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Tipo (Planejado ou Realizado) :
			</td>
			<td>
				<input type="radio" name="serplanejado" id="serplanejadoP" value="P" checked="checked" onclick="mostraCampoTipo();"/> Planejado
				<input type="radio" name="serplanejado" id="serplanejadoR" value="R" 	               onclick="mostraCampoTipo();"/> Realizado
			</td>
		</tr>
		<tr id="trSerqtde20" style="display:none">
			<td class="SubTituloDireita"> Novos Docentes 20 H :
			</td>
			<td>
				<?= campo_texto( 'serqtde20', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="serqtde20"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr id="trSerqtde40" style="display:none">
			<td class="SubTituloDireita"> Novos Docentes 40 H:
			</td>
			<td>
				<?= campo_texto( 'serqtde40', 'N', 'S', '', 25, 25, '[###.]###', '','','','','id="serqtde40"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
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
			<td id="tdServidor">
<?php 

	echo listaServidor($entidcampus);
	
?>
			</td>
		</tr>
	</table>
</form>
</div>
<script>

function carregaServidor( dados ){

	var serid   = $('serid');
	serid.value = dados.serid; 
	
	var serano   = $('serano');
	serano.value = dados.serano;
	
	var sersemestre1 = $('sersemestre1');
	var sersemestre2 = $('sersemestre2');
	if( dados.sersemestre == '1' ){
		sersemestre1.checked = true;
	}else{
		sersemestre2.checked = true;
	}
	
	var serqtdde   = $('serqtdde');
	serqtdde.value = dados.serqtdde;
	
	var serplanejadoP = $('serplanejadoP');
	var serplanejadoR = $('serplanejadoR');
	if( dados.serplanejado == 'P' ){
		serplanejadoP.checked = true;
	}else{
		serplanejadoR.checked = true;
	}
	
	mostraCampoTipo();
	
	var serqtde20   = $('serqtde20');
	serqtde20.value = dados.serqtde20;
	
	var serqtde40   = $('serqtde40');
	serqtde40.value = dados.serqtde40;

}

function excluirServidor( serid ){

	if(confirm('Realmente deseja excluir este registro?')){
		var td    = $('tdServidor');
		var entid = $('entid').value;
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: '&req=excluir&serid=' + serid + '&entid=' + entid,
			onComplete: function(res){
				alert('Registro Excluido.');
				td.innerHTML = res.responseText;
			}
		});
	}

}

function mostraCampoTipo (){

	var serplanejadoR = $('serplanejadoR');
	var tr20		  = $('trSerqtde20');
	var tr40		  = $('trSerqtde40');
	var serqtde20     = $('serqtde20');
	var serqtde40     = $('serqtde40');
	
	if( serplanejadoR.checked ){
		tr20.style.display = "table-row";
		tr40.style.display = "table-row";
	}else{
		tr20.style.display = "none";
		tr40.style.display = "none";
		serqtde20.value    = "";
		serqtde40.value    = "";
	}
	
}

function validaForm(){

	var ano  = document.getElementById('serano');
	
	var semestre1 = document.getElementById('sersemestre1');
	var semestre2 = document.getElementById('sersemestre2');
	
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


