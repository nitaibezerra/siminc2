<?php
function excluirArquivo2( $quest ){
	
	global $db;
	
	if( $quest['tbrid'] != '' ){
		$sql = "UPDATE academico.tabelaresposta SET arqid = NULL WHERE tbrid = ".$quest['tbrid'];
		$db->executar($sql);
		$db->commit();
	}
	echo "<script>
			window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=".$quest['pg']."'
		  </script>";
}
function excluirArquivo( $quest ){
	
	global $db;
	
	$sql = "DELETE FROM academico.anexosquestionario WHERE arqid = ".$quest['arqid'];
	$db->executar($sql);
	$db->commit();
	echo "<script>
			window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=4'
		  </script>";
}
function excluirLinhaTabela( $request ){
	
	global $db;
	
	extract($request);
	
	if( $tbrid != '' ){
		$sql = "DELETE FROM academico.tabelavalor
				WHERE tbrid in (SELECT tbrid FROM academico.tabelaresposta WHERE perid = $perid AND entid = $entid);
				DELETE FROM academico.tabelaresposta 
				WHERE perid = $perid AND entid = $entid AND tbrid = ".$tbrid.";";
		$db->executar($sql);
		$db->commit();
	}
}

function gravar2( $request ){
	
	global $db;
	
	extract($request);
	
	$entid = $_SESSION['academico']['entidcampus'];
	$url = $_REQUEST['url'] ? "'".$_REQUEST['url']."'" : 'window.location';
	
	foreach( $_FILES as $file ){
		$t = $file['name'] != '' ? $file : $t;
	}
	## UPLOAD DE ARQUIVO
	if( $t['name'] != '' ){
		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		if( $_REQUEST['tabela'] ){
			foreach( $_FILES as $k => $arquivo ){
				if( $arquivo['name'] != '' ){
					$id = explode('_', $k);
					$campos	= array("perid"	 => $id[1],
									"entid"	 => $entid,
									"usucpf" => "'".$_SESSION['usucpf']."'");
					$file = new FilesSimec("anexosquestionario", $campos, 'academico');
					if($_FILES[$k]){	
						$arquivoSalvo = $file->setUpload('Anexo consolidacao entid = '.$entid,$k,false);	
						if( $arquivoSalvo ){
							$arqid = $file->getIdArquivo();
							$sql = "UPDATE academico.tabelaresposta SET
										arqid = ".$arqid."
									WHERE
										tbrid = ".$id[2];
							$db->executar($sql);
							$db->commit();
						}
					}
				}
			}
		}else{
			foreach( $_FILES as $k => $arquivo ){
				if( $arquivo['name'] != '' ){
					$id = explode('_', $k);
					$campos	= array("perid"	 => $id[1],
									"entid"	 => $entid,
									"usucpf" => "'".$_SESSION['usucpf']."'");
					$file = new FilesSimec("anexosquestionario", $campos, 'academico');
					if($_FILES[$k]){	
						$arquivoSalvo = $file->setUpload('Anexo consolidacao entid = '.$entid);	
					}
				}
			}
		}
	}
	
	if( $tipoSalvar == 'resposta' ){
		$resdescricao = $resdescricao ? $resdescricao : Array();
		foreach( $resdescricao as $perid => $resposta  ){
			$sql .= "DELETE FROM academico.resposta WHERE entid = $entid AND perid = $perid;";
			$resjustificativa[$perid] = mb_detect_encoding($resjustificativa[$perid], "UTF-8", true) ? utf8_decode($resjustificativa[$perid]) : $resjustificativa[$perid] ;
			$sql .= "INSERT INTO academico.resposta( perid, entid, resdescricao, resjustificativa, usucpf)
		   			VALUES ( $perid, $entid, '$resposta', '".substr($resjustificativa[$perid], 0, 1499)."', '".$_SESSION['usucpf']."');";
		}
		if( $sql != '' ){
			if($db->executar($sql)){
				$db->commit();
				$script = $url == 'window.location' ? "<script>alert('Dados gravados.');window.location = $url</script>" : "<script>window.location = $url</script>";
				echo $script;
			}else{
				$db->rollback();
				echo "<script>alert('Erro ao gravar.');window.location = window.location</script>";
			}
		}
		$script = $url == 'window.location' ? "<script>alert('Dados gravados.');window.location = $url</script>" : "<script>window.location = $url</script>";
		echo $script;
	}elseif( $tipoSalvar == 'justivifativa' ){
		$resjustificativa = $resjustificativa ? $resjustificativa : Array();
		foreach( $resjustificativa as $perid => $justivifativa  ){
			$sql .= "DELETE FROM academico.resposta WHERE entid = $entid AND perid = $perid;";
			$resjustificativa[$perid] = mb_detect_encoding($resjustificativa[$perid], "UTF-8", true) ? utf8_decode($resjustificativa[$perid]) : $resjustificativa[$perid] ;
			$sql .= "INSERT INTO academico.resposta( perid, entid, resjustificativa, usucpf)
		   			VALUES ( $perid, $entid, '".substr($resjustificativa[$perid], 0, 1499)."', '".$_SESSION['usucpf']."');";
		}
		if($db->executar($sql)){
			$db->commit();
			$script = $url == 'window.location' ? "<script>alert('Dados gravados.');window.location = $url</script>" : "<script>window.location = $url</script>";
			echo $script;
		}else{
			$db->rollback();
			echo "<script>alert('Erro ao gravar.');window.location = window.location</script>";
		}
	}elseif( $tipoSalvar == 'tabela' ){
		$perid = $_REQUEST['perid'];
		$tbrids = Array();
		$tbrdemanda = $tbrdemanda ? $tbrdemanda : Array();
		foreach( $tbrdemanda as $tbtid => $valores ){
			foreach( $valores as $linha => $demanda ){
				$area = str_replace(Array('.',','),Array('','.'),$tbrarea[$tbtid][$linha]);
				$area = $area != '' ? $area : 0;
				$aluno = str_replace(Array('.',','),Array('','.'),$tbraluno[$tbtid][$linha]);
				$aluno = $aluno != '' ? $aluno : 0;
				$projeto = $tbfprojeto[$tbtid][$linha] ? $tbfprojeto[$tbtid][$linha] : 'NULL';
				$tbrid = $tbrid2[$tbtid][$linha];
				if( $tbrid != '' ){
					$sql = "UPDATE academico.tabelaresposta SET
					            tbrdemanda = '$demanda', 
					            tbrarea = $area, 
					            tbraluno = $aluno, 
					            tbrprojeto = $projeto
					        WHERE
					        	tbrid = $tbrid
					        RETURNING tbrid;";
					$tbrid = $db->pegaUm($sql);
				}else{
					$sql = "INSERT INTO academico.tabelaresposta(
					            perid, tbtid, entid, tbrdemanda, tbrarea, tbraluno, tbrprojeto, usucpf)
					    	VALUES ($perid, $tbtid, $entid, '$demanda', $area, $aluno, $projeto, '".$_SESSION['usucpf']."') 
					    	RETURNING tbrid;";
					$tbrid = $db->pegaUm($sql);
				}
				$tbrids[] = $tbrid;
				$sqlValor = '';
				if( $tbrid != '' ){
					$sql = "DELETE FROM academico.tabelavalor
							WHERE tbrid = $tbrid";
					$db->executar($sql);
					if( is_array($tbvcusteio[$tbtid][$linha]) ){
						foreach( $tbvcusteio[$tbtid][$linha] as $ano => $custeio ){
							$custeio = str_replace(Array('.',','),Array('','.'),$custeio);
							$custeio = $custeio != '' ? $custeio : 0;
							$capital = str_replace(Array('.',','),Array('','.'),$tbvcapital[$tbtid][$linha][$ano]);
							$capital = $capital != '' ? $capital : 0;
							$sqlValor .= "INSERT INTO academico.tabelavalor(
							            	tbrid, tbvcusteio, tbvcapital, tbvano)
							    		VALUES ( $tbrid, $custeio, $capital, $ano);";
						}
					}else{
						foreach( $tbvcapital[$tbtid][$linha] as $ano => $capital ){
							$capital = str_replace(Array('.',','),Array('','.'),$capital);
							$capital = $capital != '' ? $capital : 0;
							$custeio = str_replace(Array('.',','),Array('','.'),$tbvcusteio[$tbtid][$linha][$ano]);
							$custeio = $custeio != '' ? $custeio : 0;
							$sqlValor .= "INSERT INTO academico.tabelavalor(
							            	tbrid, tbvcusteio, tbvcapital, tbvano)
							    		VALUES ( $tbrid, $custeio, $capital, $ano);";
						}
					}
				}
				if( $sqlValor != '' ){
					$db->executar($sqlValor);
				}
			}
		}
		$camposZerados = false;
		$db->commit();
		$script = $url == 'window.location' ? "<script>alert('Dados gravados.');window.location = $url</script>" : "<script>window.location = $url</script>";
		echo $script;
	}else{
		$script = $url == 'window.location' ? "<script>alert('Dados gravados.');window.location = $url</script>" : "<script>window.location = $url</script>";
		echo $script;
	}
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

if( $_REQUEST['arqid'] ){
	$caminho = APPRAIZ.'arquivos/academico/'.floor($_REQUEST['arqid']/1000).'/'.$_REQUEST['arqid'];
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$_REQUEST['arqid'];
	$arquivo = $db->pegaLinha($sql);
	$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	echo "<script>
			window.location = 'geral/baixar_arquivo.php?url={".$caminho."}&arqtipo=".$arquivo['arqtipo']."&filename=".$filename."'
		  </script>";
    exit;
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";

$_SESSION['academico']['questPg'] = $_REQUEST['questPg'] ? $_REQUEST['questPg'] : '1';

$prox = $_SESSION['academico']['questPg']+1;
$ant = $_SESSION['academico']['questPg']-1;

$_SESSION['academico']['entidcampus'] = $_REQUEST['entidcampus'] ? $_REQUEST['entidcampus'] : $_SESSION['academico']['entidcampus'];
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entidcampus'];

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entid'];
$orgid = $_SESSION['academico']['orgid'];

$existecampus = academico_existeentidade( $_SESSION['academico']['entidcampus'] );

if( !$existecampus ){
	include_once(APPRAIZ."academico/modulos/principal/consolidacaoExpansao/consolidacaoExp_listaCampus.inc" );
	die;
}

$docid = pegaDocidConsolidacao();
// instancia o objeto do módulo 
$academico = new academico();

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   PERFIL_CADASTROGERAL, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
					   PERFIL_REITOR,
					   PERFIL_ALTA_GESTAO,
					   PERFIL_ASSESSORIA_ALTA_GESTAO);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

$bloqueado = $permissoes['gravar'] ? false :  true;
$bloq = '';
$hide = '';
if($bloqueado) {
	$bloq = 'disabled="disabled"';
	$hide = 'hide';
}
// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$unidade = ($_REQUEST['entidunidade'] || $_SESSION['academico']['entid']) ? $entid : $autoriazacaoconcursos->buscaentidade($entid);

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

if($unidade) {
	$_SESSION['academico']['entid'] 		= $unidade;
	$_SESSION['academico']['entidadenivel'] = 'unidade';
	$_SESSION['sig_var']['entid'] 			= $_SESSION['academico']['entid'];
	$_SESSION['sig_var']['iscampus'] 		= 'nao';
	
	$edtdsc = $db->pegaUm("SELECT entd.edtdsc FROM academico.entidadedetalhe entd WHERE entd.entid='".$_SESSION['sig_var']['entid']."'");
	$edtdsc = simec_htmlspecialchars(str_replace(array("\r\n","\n\r","\r","\n"), '\n', $edtdsc), ENT_QUOTES);
}

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$unidade) {
	echo "<script>alert('Unidade não selecionada.');window.location='?modulo=inicio&acao=C';</script>";
	exit;
}

$url = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A';
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Programa de Consolidação das IFES - Campus", "");

$orgid = $_SESSION['academico']['orgid'];
$orgao = "Educação Superior";

//Cabeçalho Unidade
$sql = "SELECT 
			ent.entid as unidadeorcid, 
			ent.entnome as unidadeorc, 
			ende.estuf, 
			mundescricao,
			entunicod 
		FROM 
			entidade.entidade ent
		LEFT JOIN entidade.endereco 	ende ON ende.entid = ent.entid 
		LEFT JOIN territorios.municipio mun  ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
		WHERE 
			ent.entid = '". $entid ."' 
		ORDER BY 
			ent.entnome";
	
$dados = $db->pegaLinha( $sql );

$cabecalho = '';
if($dados) {
	$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
	. "<tr>"
	. "<td class='SubTituloDireita' width='250px;'>Tipo Ensino:</td><td>".$orgao."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>Instituição:</td><td>".$dados['unidadeorc']." - ".$dados['entunicod']."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>UF / Munícipio:</td><td>".$dados['estuf']." / ".$dados['mundescricao']."</td>"
	. "</tr>"
	. "</table>";
} else {
	$cabecalho .=("<script>
			alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
			window.location='?modulo=inicio&acao=C';
		 </script>");
}
		   
echo $cabecalho;	
//FIM Cabeçalho Unidade

//Cabeçalho Campus
$sql = "SELECT 
			ent.entid as unidadeorcid, 
			ent.entnome as unidadeorc, 
			ende.estuf, 
			mundescricao,
			entunicod 
		FROM 
			entidade.entidade ent
		LEFT JOIN entidade.endereco 	ende ON ende.entid = ent.entid 
		LEFT JOIN territorios.municipio mun  ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
		WHERE 
			ent.entid = '". $entidcampus ."' 
		ORDER BY 
			ent.entnome";
	
$dados = $db->pegaLinha( $sql );

$cabecalho = '';
if($dados) {
	$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
	. "<tr>"
	. "<td class='SubTituloDireita' width='250px;'>Campus:</td><td>".$dados['unidadeorc']."</td>"
	. "</tr>"
	. "<tr>"
	. "<td class='SubTituloDireita'>UF / Munícipio:</td><td>".$dados['estuf']." / ".$dados['mundescricao']."</td>"
	. "</tr>"
	. "</table>";
} else {
	$cabecalho .=("<script>
					alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
					window.location='?modulo=inicio&acao=C';
				 </script>");
}
		   
echo $cabecalho;	
//FIM Cabeçalho Campus

?>
<form name="formulario" method="POST" action="" enctype="multipart/form-data">
	<input type="hidden" name="url" value=""/>
	<?php 
	$bloq = verificaEstadoPreenchimentoEntidade( $_SESSION['academico']['entidcampus'] );
	$bloq = $bloq ? 'S' : 'N';
	include_once(APPRAIZ."academico/modulos/principal/consolidacaoExpansao/consolidacaoExp_questCamp_pg".$_SESSION['academico']['questPg'].".inc" );
	?>
	<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td width="100%" align="center">
				<input type="button" value="Anterior" id="btnAnt"/>
				<input type="button" value="Gravar"   id="btnGravar" <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/>
				<? if( $prox <= 12 ){?>
				<input type="button" value="Proximo" id="btnProx"/>
				<? } ?>
				<input type="button" value="Responder outro campus" id="btfCampus" style="float:right"/>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function(){
	$('#btnGravar').click(function(){
//		var erro = false;
//		var temp_name;
//		var temp_check;
//		var gato;
//		$('[name*="tbfprojeto"]').each(function(){
//			if( $(this).attr('name') == temp_name ){
//				if( !$(this).attr('checked') && !temp_check ){
//					erro = true;
//					$(this).focus();
//					return false;
//				}
//			}
//			temp_name = $(this).attr('name');
//			temp_check = $(this).attr('checked');
//		});
//		$('[name*="resdescricao"]').each(function(){
//			gato = $(this).attr('name');
//			if( gato.indexOf('16') < 0 && gato.indexOf('17') < 0 ){
//				if( $(this).attr('name') == temp_name ){
//					if( !$(this).attr('checked') && !temp_check ){
//						erro = true;
//						$(this).focus();
//						return false;
//					}
//				}
//				temp_name = $(this).attr('name');
//				temp_check = $(this).attr('checked');
//			}
//			else{
//				if( $('[name="resdescricao[15]"]').attr('checked') ){
//					if( $(this).attr('name') == temp_name ){
//						if( !$(this).attr('checked') && !temp_check ){
//							erro = true;
//							$(this).focus();
//							return false;
//						}
//					}
//					temp_name = $(this).attr('name');
//					temp_check = $(this).attr('checked');
//				}
//			}
//		});
//		if( erro ){
//			alert('Campo obrigatório.');
//			return false;
//		}
//		$('.obrigatorio').each(function(){
//			if( $(this).val() == '' ){
//				erro = true;
//				$(this).focus();
//				return false;
//			}
//		});
//		if( erro ){
//			alert('Campo obrigatório.');
//			return false;
//		}
		$('[name="req"]').val('gravar2');
		$('[name="formulario"]').submit();
	});
	$('[name*="tbvcusteio"],[name*="tbvcapital"],[name*="tbraluno"]').live('blur',function(){
		$(this).val(mascaraglobal('[.###],##',$(this).val()));
	})
	$('#btnAnt').click(function(){
		<?php if( $ant == 0 ){?>
			<?php //if( $bloq == 'S' ){?>
//			$('[name="url"]').val('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A'); 
			<?php //}else{?>
			window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A';
			<?php //}?>
		<?php }else{?>
			<?php //if( $bloq == 'S' ){?>
//			$('[name="url"]').val('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=<?=$ant ?>');
			<?php //}else{?>
			window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=<?=$ant ?>';
			<?php //}?>
		<?php }?>
		<?php if( $bloq == 'S' ){?>
//		$('#btnGravar').click();
		<?php }?>
	});
	<?php if( $prox <= 12 ){?>
	$('#btnProx').click(function(){
		<?php //if( $bloq == 'S' ){?>
//		$('[name="url"]').val('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=<?=$prox ?>');
//		$('#btnGravar').click();
		<?php //}else{?>
		window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=<?=$prox ?>';
		<?php //}?>
	});
	<?php }?>
	$('#btfCampus').click(function(){
		<?php //if( $bloq == 'S' ){?>
//		$('[name="url"]').val('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A&campus=true');
//		$('#btnGravar').click();
		<?php //}else{?>
		window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A&campus=true';
		<?php //}?>
	});
	$('.btfAba').click(function(){
		<?php //if( $bloq == 'S' ){?>
//		$('[name="url"]').val($(this).attr('id'));
//		$('#btnGravar').click();
		<?php //}else{?>
		window.location = $(this).attr('id');
		<?php //}?>
	});
});

</script>