<?php 

function comboModalidade( $nivid ){
	
	global $db;
	
	$sql = "SELECT
				modid as codigo,
				modnome as descricao
			FROM
				academico.catalogomodalidade cm
			WHERE
				modstatus = 'A' AND
				nivid = {$nivid}
			ORDER BY
				codigo";
				
	$db->monta_combo('modid',$sql,'S', 'Selecione uma Opção...', '','','','','S','modid' );
	
}

if(	$_REQUEST['req'] == 'modalidade' ){
	
	echo comboModalidade( $_REQUEST['nivid'] );
	die();
}

function listaCursos ( $entid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				oc.ofcid,
				oc.eixid,
				ce.eixnome,
				cm.nivid,
				cn.nivnome,
				oc.modid,
				cm.modnome,
				oc.ofcnomecurso,
				oc.ofcsemestre,
				oc.ofcnumvagas,
				oc.ofcnumturmas,
				oc.ofcturnom,
				oc.ofcturnot,
				oc.ofcturnon,
				oc.ofccargahoraria,
				oc.ofcorcamento,
				oc.ofcpresencial,
				oc.ofcsemestresaula,
				oc.ofcsemestresestagio,
				oc.ofcanoprimeiraoferta,
				oc.ofcanoultimaoferta
			FROM
				academico.ofertacursos oc
			LEFT JOIN
				academico.catalogoeixo ce ON ce.eixid = oc.eixid
			LEFT JOIN
				academico.catalogomodalidade cm ON cm.modid = oc.modid 
			LEFT JOIN
				academico.catalogonivel cn ON cn.nivid = cm.nivid
			WHERE
				entidcampus = {$entid}
			ORDER BY
				ce.eixnome, cn.nivnome, cm.modnome, ofcnomecurso, ofcsemestre";
//	ver($sql);
	$cursos = $db->carregar($sql);
	
?>
				<table width="98%" class="Tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: rgb(220, 220, 220);">
						<td width="10%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ação </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Eixo </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Nível </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Modalidade </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Curso </b>
						</td>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
							<b> Semestre </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Vagas </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Turmas </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Turno </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Carga Horária </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Orçamento </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Presencial </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Semestres de Aula </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Semestres de Estágio </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ano da Primeira Oferta </b>
						</td>
						<td align="center" valign="top" style="border-bottom: 1px solid rgb(192, 192, 192);" class="title">
							<b> Ano da Última Oferta </b>
						</td>
					</tr>
<?php 
	$i=0;
	if( is_array($cursos) ){
		foreach( $cursos as $curso ){
			
			if( $i % 2 == 1 ){
				$cor = 'rgb(235, 235, 235);';
				$i++;
			}else{
				$cor = 'rgb(255, 255, 255);';
				$i++;
			}
			$curso['ofcnomecurso'] = utf8_encode($curso['ofcnomecurso']);
			$json = simec_json_encode($curso);
			$curso['ofcnomecurso'] = utf8_decode($curso['ofcnomecurso']);
?>
					<tr style="background-color: <?=$cor; ?>">
						<td align="center">
							<img border="0" onclick='carregaCurso(<?=$json;?>);'           style="cursor: pointer;" title="Editar" src="/imagens/alterar.gif">
							<img border="0" onclick='excluirCurso(<?=$curso['ofcid'];?>);' style="cursor: pointer;" title="Editar" src="/imagens/excluir.gif">
						</td>
						<td align="center">
							<?=$curso['eixnome'] ?>
						</td>
						<td align="center">
							<?=$curso['nivnome'] ?>
						</td>
						<td align="center">
							<?=$curso['modnome'] ?>
						</td>
						<td align="center">
							<?=$curso['ofcnomecurso'] ?>
						</td>
						<td align="center">
							<?=$curso['ofcsemestre'] ?>°
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcnumvagas'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcnumturmas'],0,',','.') ?>
						</td>
						<td align="center">
							<?=$curso['ofcturnom'] == 't' ? 'Matutino<br>'   : ''; ?>
							<?=$curso['ofcturnot'] == 't' ? 'Vespertino<br>' : ''; ?>
							<?=$curso['ofcturnon'] == 't' ? 'Noturno<br>'    : ''; ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofccargahoraria'],0,',','.') ?>
						</td>
						<td align="center">
							<?=$curso['ofcorcamento'] == 't' ? 'Sim' : 'Não'; ?>
						</td>
						<td align="center">
							<?=$curso['ofcpresencial'] == 't' ? 'Sim' : 'Não'; ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcsemestresaula'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcsemestresestagio'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcanoprimeiraoferta'],0,',','') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($curso['ofcanoultimaoferta'],0,',','') ?>
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

function salvaCursos ( $dados ){
	
	global $db;
    
//	dbg($dados,1);
	
	//Tratamento de valores do Padrão brasileiro para padrão Americano(do Banco)
	$dados['ofcnumvagas']   	   = str_replace(".","",$dados['ofcnumvagas']);
	$dados['ofcnumturmas']   	   = str_replace(".","",$dados['ofcnumturmas']);
    $dados['ofccargahoraria']      = str_replace(".","",$dados['ofccargahoraria']);
	$dados['ofcsemestresaula']     = str_replace(".","",$dados['ofcsemestresaula']);
	$dados['ofcsemestresestagio']  = str_replace(".","",$dados['ofcsemestresestagio']);
	
    //Tratamento para campos vazios
    $dados['ofcnumvagas']     	   = $dados['ofcnumvagas']  	    ? $dados['ofcnumvagas']   				: 'null';
    $dados['ofcnumturmas']    	   = $dados['ofcnumturmas'] 	    ? $dados['ofcnumturmas']  				: 'null';
    $ofcturnoInCol		      	   = $dados['ofcturno']     	    ? $dados['ofcturno'].', ' 				: '';
    $ofcturnoInVal		      	   = $dados['ofcturno']     	    ? 'true ,'      			  				: '';
//    $ofcturnoUp		   	      	   = $dados['ofcturno']     	    ? $dados['ofcturno'].' = true , ' 	: '';
//	switch ( $dados['ofcturno'] ) {
//	    case 'ofcturnom':
//	        $ofcturnoUp = "ofcturnom = true, ofcturnot = false, ofcturnon = false,";
//	        break;
//	    case 'ofcturnot':
//	        $ofcturnoUp = "ofcturnom = false, ofcturnot = true, ofcturnon = false,";
//	        break;
//	    case 'ofcturnon':
//	        $ofcturnoUp = "ofcturnom = false, ofcturnot = false, ofcturnon = true,";
//	        break;
//	}
	$dados['ofcturnom'] = $dados['ofcturnom'] ? 'true' : 'false';
	$dados['ofcturnot'] = $dados['ofcturnot'] ? 'true' : 'false';
	$dados['ofcturnon'] = $dados['ofcturnon'] ? 'true' : 'false';
	
    $dados['ofccargahoraria'] 	   = $dados['ofccargahoraria'] 	    ? $dados['ofccargahoraria']  			: 'null';
    $dados['ofcsemestresaula'] 	   = $dados['ofcsemestresaula']     ? $dados['ofcsemestresaula']  			: 'null';
    $dados['ofcsemestresestagio']  = $dados['ofcsemestresestagio']  ? $dados['ofcsemestresestagio']  		: 'null';
    $dados['ofcanoprimeiraoferta'] = $dados['ofcanoprimeiraoferta'] ? $dados['ofcanoprimeiraoferta']  		: 'null';
    $dados['ofcanoultimaoferta']   = $dados['ofcanoultimaoferta']   ? $dados['ofcanoultimaoferta']  		: 'null';
    
    //Se possuir ofcid, atualiza, se não, insere
	if( $dados['ofcid'] ){
		
		$sql = "UPDATE academico.ofertacursos
			    SET 
			    	eixid 				 = {$dados['eixid']}, 
			    	modid 				 = {$dados['modid']}, 
			       	entidcampus 		 = {$dados['entidcampus']},
			    	ofcnomecurso 		 = '".utf8_encode($dados['ofcnomecurso'])."', 
			        ofcsemestre 		 = {$dados['ofcsemestre']}, 
			        ofcnumvagas 		 = {$dados['ofcnumvagas']}, 
			        ofcnumturmas 		 = {$dados['ofcnumturmas']}, 
			        --{$ofcturnoUp} 
			        ofcturnom			 = {$dados['ofcturnom']},
			        ofcturnot			 = {$dados['ofcturnot']},
			        ofcturnon			 = {$dados['ofcturnon']},
			       	ofccargahoraria 	 = {$dados['ofccargahoraria']}, 
			       	ofcorcamento 		 = {$dados['ofcorcamento']}, 
			       	ofcpresencial 		 = {$dados['ofcpresencial']}, 
			       	ofcsemestresaula 	 = {$dados['ofcsemestresaula']}, 
			       	ofcsemestresestagio  = {$dados['ofcsemestresestagio']}, 
			       	ofcanoprimeiraoferta = {$dados['ofcanoprimeiraoferta']}, 
			       	ofcanoultimaoferta   = {$dados['ofcanoultimaoferta']}
			 	WHERE 
			 		ofcid = {$dados['ofcid']}";
		
	}else{
		
		$sql = "INSERT INTO academico.ofertacursos(
			            eixid,
			            modid,
			            entidcampus,  
			            ofcnomecurso, 
			            ofcsemestre, 
			            ofcnumvagas, 
			            ofcnumturmas, 
			            --{$ofcturnoInCol}
			            ofcturnom,
			            ofcturnot,
			            ofcturnon,
			            ofccargahoraria, 
			            ofcorcamento, 
			            ofcpresencial, 
			            ofcsemestresaula, 
			            ofcsemestresestagio, 
			            ofcanoprimeiraoferta, 
			            ofcanoultimaoferta )
			    VALUES ({$dados['eixid']}, 
			    		{$dados['modid']}, 
			    		{$dados['entidcampus']}, 
			    		'{$dados['ofcnomecurso']}', 
			    		{$dados['ofcsemestre']}, 
			    		{$dados['ofcnumvagas']}, 
			    		{$dados['ofcnumturmas']}, 
			    		--{$ofcturnoInVal}
			    		{$dados['ofcturnom']},
			    		{$dados['ofcturnot']},
			    		{$dados['ofcturnon']},
			            {$dados['ofccargahoraria']}, 
			            {$dados['ofcorcamento']}, 
			            {$dados['ofcpresencial']},
			            {$dados['ofcsemestresaula']}, 
			            {$dados['ofcsemestresestagio']}, 
			            {$dados['ofcanoprimeiraoferta']}, 
			            {$dados['ofcanoultimaoferta']});";
		
	}
	
	//Se não possuir erro comita.
	if($db->executar($sql)){
		$db->commit();
		echo "<script>alert('Dados salvos com sucesso!');</script>";
	}

}


if( $_REQUEST['req'] == 'excluir' ){
	
	$sql = "DELETE FROM
				academico.ofertacursos
			WHERE
				ofcid = {$_REQUEST['ofcid']}";
	
	$db->executar($sql);
	$db->commit();
	
	echo listaCursos($_REQUEST['entid']);
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
monta_titulo( "Oferta de Cursos", "");

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
	salvaCursos( $_REQUEST );
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
<form name="frmCursos" id="frmCursos" method="post" action="" onsubmit="">

	<input type="hidden" id="entidcampus" name="entidcampus" value="<?=$entidcampus; ?>">
	<input type="hidden" id="ofcid" 	  name="ofcid" 		 value="">
	<input type="hidden" id="req"   	  name="req"   		 value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
		<tr>
			<td></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Eixo :
			</td>
			<td>
				<?php 
				$sql = "SELECT
							eixid as codigo,
							eixnome as descricao
						FROM
							academico.catalogoeixo
						WHERE 
							eixstatus = 'A'";
				
				$db->monta_combo('eixid',$sql,'S', 'Selecione uma Opção...', '','','','','S','eixid' );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Nivel :
			</td>
			<td>
				<?php 
				$sql = "SELECT
							nivid as codigo,
							nivnome as descricao
						FROM
							academico.catalogonivel
						WHERE
							nivstatus = 'A'";
				
				$db->monta_combo('nivid',$sql,'S', 'Selecione uma Opção...', 'exibeModalidade','','','','S','nivid' );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="250px"> Modalidade :
			</td>
			<td>	
				<div id="divmodalidade">
					<select id="modid" name="modid" disabled="disabled">
						<option value=""> Selecione um Nivel... </option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Nome do Curso :
			</td>
			<td>
				<?= campo_textarea( 'ofcnomecurso', 'S', 'S', '', '68', '6', '255', '' , 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Semestre :
			</td>
			<td>
				<input type="radio" name="ofcsemestre" id="ofcsemestre1" value="1" checked="checked" /> 1° Semestre
				<input type="radio" name="ofcsemestre" id="ofcsemestre2" value="2" /> 2° Semestre
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de Vagas :
			</td>
			<td>
				<?= campo_texto( 'ofcnumvagas', 'N', 'S', '', 25, 500, '[###.]###', '','','','','id="ofcnumvagas"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Número de Turmas :
			</td>
			<td>
				<?= campo_texto( 'ofcnumturmas', 'N', 'S', '', 25, 500, '[###.]###', '','','','','id="ofcnumturmas"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Turno :
			</td>
			<td>
				<input type="checkbox" name="ofcturnom" id="ofcturnom" checked="checked" /> Matutino
				<input type="checkbox" name="ofcturnot" id="ofcturnot" /> Vespertino
				<input type="checkbox" name="ofcturnon" id="ofcturnon" /> Noturno
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Carga Horária :
			</td>
			<td>
				<?= campo_texto( 'ofccargahoraria', 'N', 'S', '', 5, 6, '[###.]###', '','','','','id="ofccargahoraria"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>  Horas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Orçamento :
			</td>
			<td>
				<input type="radio" name="ofcorcamento" id="ofcorcamento1" value="true" checked="checked" /> Sim
				<input type="radio" name="ofcorcamento" id="ofcorcamento0" value="false" /> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Presencial :
			</td>
			<td>
				<input type="radio" name="ofcpresencial" id="ofcpresencial1" value="true" checked="checked" /> Sim
				<input type="radio" name="ofcpresencial" id="ofcpresencial0" value="false" /> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Semestres de Aula :
			</td>
			<td>
				<?= campo_texto( 'ofcsemestresaula', 'N', 'S', '', 5, 6, '[###.]###', '','','','','id="ofcsemestresaula"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
				Semestre(s)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Semestres de Estágio :
			</td>
			<td>
				<?= campo_texto( 'ofcsemestresestagio', 'N', 'S', '', 5, 6, '[###.]###', '','','','','id="ofcsemestresestagio"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
				Semestre(s)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Ano da Primeira Oferta :
			</td>
			<td>
				<?= campo_texto( 'ofcanoprimeiraoferta', 'N', 'S', '', 4, 6, '####', '','','','','id="ofcanoprimeiraoferta"','','','this.value=mascaraglobal(\'####\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Ano da Última Oferta :
			</td>
			<td>
				<?= campo_texto( 'ofcanoultimaoferta', 'N', 'S', '', 4, 6, '####', '','','','','id="ofcanoultimaoferta"','','','this.value=mascaraglobal(\'####\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td> 
			</td>
			<td id="tdAlunos" style="display: none" >
				<input type="button" value="Cadastrar Alunos" onclick="abrePopupAlunos();" ></input>
			</td>
		</tr>
		<tr>
			<td> 
			</td>
			<td>
				<?php if( $permissoes['gravar'] ){ ?>
					<input type="button" value="Inserir" id="gravar" onclick="validaForm();"/>
				<?php } ?>
				<input type="button" value="Limpar"  id="limpar" onclick="limparCampos();" style="display:none"/>
			</td>
		</tr>
		<tr>
			<td></td>
			<td id="tdServidor">
<?php 

	echo listaCursos($entidcampus);
	
?>
			</td>
		</tr>
	</table>
</form>
<script>

var entid = <?=$entidcampus ?>

function abrePopupAlunos(){

	var ofcid    = $('ofcid').value;
	var tdAlunos = $('tdAlunos');
	
	if( ofcid == '' ){
		alert('Escolha um curso!');
		tdAlunos.style.display = 'none';
	}else{
		newwindow=window.open('?modulo=principal/popup/popupCadastrodeAlunos&acao=A&ofcid='+ofcid,'Cadastro de Alunos','height=500,width=700');
		if (window.focus) {newwindow.focus()}
		return false;
	}
	
}

function carregaCurso( dados ){

	var tdAlunos = $('tdAlunos');
	tdAlunos.style.display = 'block';
	
	var ofcid   = $('ofcid');
	ofcid.value = dados.ofcid;
	 
	var eixid   = $('eixid');
	eixid.value = dados.eixid; 
	
	var nivid   = $('nivid');
	nivid.value = dados.nivid;

	exibeModalidade( dados.nivid );
	
	var modid   = $('modid');
	modid.value = dados.modid;

	var ofcnomecurso   = $('ofcnomecurso');
	ofcnomecurso.value = dados.ofcnomecurso;

	var ofcsemestre1   = $('ofcsemestre1');
	var ofcsemestre2   = $('ofcsemestre2');
	if( dados.ofcsemestre == '1' ){
		ofcsemestre1.checked = true;
	}else{
		ofcsemestre2.checked = true;
	}

	var ofcnumvagas   = $('ofcnumvagas');
	ofcnumvagas.value = dados.ofcnumvagas;

	var ofcnumturmas   = $('ofcnumturmas');
	ofcnumturmas.value = dados.ofcnumturmas;
	
	var ofcturnom = $('ofcturnom');
	var ofcturnot = $('ofcturnot');
	var ofcturnon = $('ofcturnon');
	if( dados.ofcturnom == 't' ){
		ofcturnom.checked = true;
	}else{
		ofcturnom.checked = false;
	}
	if( dados.ofcturnot == 't' ){
		ofcturnot.checked = true;
	}else{
		ofcturnot.checked = false;
	}
	if( dados.ofcturnon == 't' ){
		ofcturnon.checked = true;
	}else{
		ofcturnon.checked = false;
	}
	
	var ofccargahoraria   = $('ofccargahoraria');
	ofccargahoraria.value = dados.ofccargahoraria;
	
	var ofcorcamento1 = $('ofcorcamento1');
	var ofcorcamento0 = $('ofcorcamento0');
	if( dados.ofcorcamento == 't' ){
		ofcorcamento1.checked = true;
	}else{
		ofcorcamento0.checked = true;
	}
	
	var ofcpresencial1 = $('ofcpresencial1');
	var ofcpresencial0 = $('ofcpresencial0');
	if( dados.ofcpresencial == 't' ){
		ofcpresencial1.checked = true;
	}else{
		ofcpresencial0.checked = true;
	}
	
	var ofcsemestresaula   = $('ofcsemestresaula');
	ofcsemestresaula.value = dados.ofcsemestresaula;
	
	var ofcsemestresestagio   = $('ofcsemestresestagio');
	ofcsemestresestagio.value = dados.ofcsemestresestagio;
	
	var ofcanoprimeiraoferta   = $('ofcanoprimeiraoferta');
	ofcanoprimeiraoferta.value = dados.ofcanoprimeiraoferta;
	
	var ofcanoultimaoferta   = $('ofcanoultimaoferta');
	ofcanoultimaoferta.value = dados.ofcanoultimaoferta;

	var gravar = $('gravar');
	gravar.value = "Salvar Alterações";
	
	var limpar = $('limpar');
	limpar.style.display = "";
}

function exibeModalidade( nivid ){

	var div = $('divmodalidade')
	
	if( nivid != '' ){
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=modalidade&nivid=' + nivid,
			onComplete: function(res){
				div.innerHTML = res.responseText;
			}
		});
	}else{
		div.innerHTML = '<select id="modid" name="modid" disabled="disabled">'+
							'<option value=""> Selecione um Nivel... </option>'+
			 			'</select>';
	}

}

function excluirCurso( ofcid ){

	var td = $('tdServidor')
	
	if( confirm('Realmente deseja excluir este curso?') ){
		return new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=excluir&ofcid=' + ofcid + '&entid=' + entid,
			onComplete: function(res){
				td.innerHTML = res.responseText;
				alert('Curso excluído!');
			}
		});
	}

}

function limparCampos(){

	var tdAlunos = $('tdAlunos');
	tdAlunos.style.display = 'none';
	
	var ofcid   = $('ofcid');
	ofcid.value = '';
	 
	var eixid   = $('eixid');
	eixid.value = ''; 
	
	var nivid   = $('nivid');
	nivid.value = '';

	var div = $('divmodalidade')
	div.innerHTML = '<select id="modid" name="modid" disabled="disabled">'+
						'<option value=""> Selecione um Nivel... </option>'+
		 			'</select>';
	
	var modid   = $('modid');
	modid.value = '';

	var ofcnomecurso   = $('ofcnomecurso');
	ofcnomecurso.value = '';

	var ofcsemestre1   = $('ofcsemestre1');
	var ofcsemestre2   = $('ofcsemestre2');
	ofcsemestre1.checked = true;
	ofcsemestre2.checked = false;

	var ofcnumvagas   = $('ofcnumvagas');
	ofcnumvagas.value = '';

	var ofcnumturmas   = $('ofcnumturmas');
	ofcnumturmas.value = '';
	
	var ofcturnom = $('ofcturnom');
	var ofcturnot = $('ofcturnot');
	var ofcturnon = $('ofcturnon');
	ofcturnom.checked = false;
	ofcturnot.checked = false;
	ofcturnon.checked = false;
	
	var ofccargahoraria   = $('ofccargahoraria');
	ofccargahoraria.value = '';
	
	var ofcorcamento1 = $('ofcorcamento1');
	var ofcorcamento0 = $('ofcorcamento0');
	ofcorcamento1.checked = true;
	ofcorcamento0.checked = false;
	
	var ofcpresencial1 = $('ofcpresencial1');
	var ofcpresencial0 = $('ofcpresencial0');
	ofcpresencial1.checked = true;
	ofcpresencial0.checked = false;
	
	var ofcsemestresaula   = $('ofcsemestresaula');
	ofcsemestresaula.value = '';
	
	var ofcsemestresestagio   = $('ofcsemestresestagio');
	ofcsemestresestagio.value = '';
	
	var ofcanoprimeiraoferta   = $('ofcanoprimeiraoferta');
	ofcanoprimeiraoferta.value = '';
	
	var ofcanoultimaoferta   = $('ofcanoultimaoferta');
	ofcanoultimaoferta.value = '';
	
	var gravar = $('gravar');
	gravar.value = "Inserir";
	
	var limpar = $('limpar');
	limpar.style.display = "";
}

function validaForm(){

	var form 		  = $('frmCursos');
	var req 		  = $('req');
	var eixid 		  = $('eixid');
	var nivid 		  = $('nivid');
	var modid 		  = $('modid');
	var ofcid 		  = $('ofcid');
	var ofcnomecurso  = $('ofcnomecurso'); 

	if( eixid.value == '' ){
		alert('Item obrigatório.');
		eixid.focus();
		return false;
	}
	
	if( nivid.value == '' ){
		alert('Item obrigatório.');
		nivid.focus();
		return false;
	}
	
	if( modid.value == '' ){
		alert('Item obrigatório.');
		modid.focus();
		return false;
	}
	
	if( ofcnomecurso.value == '' ){
		alert('Item obrigatório.');
		ofcnomecurso.focus();
		return false;
	}

	if( ofcid.value != '' ){
		if( !confirm('Deseja salvar estas alterações?') ){
			return false;
		}
	}

	var gravar = $('gravar');
	gravar.value = "Inserir";
	
	var limpar = $('limpar');
	limpar.style.display = "";
	
	req.value = 'gravar';
	form.submit();
}

</script>


