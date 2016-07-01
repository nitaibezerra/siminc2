<?
function atualizaComboMunicipio( $request ){
	global $db;

	extract($request);

	if( $estuf != '' ) $whereMuncod = " WHERE estuf = '".$estuf."' ";

	$sql = "SELECT
	muncod as codigo,
	mundescricao||' - '||estuf as descricao
	FROM
	territorios.municipio
	$whereMuncod
	ORDER BY
	2";
	echo $db->monta_combo('muncod',$sql, 'S','Selecione...','','','',200,'N', 'muncod', '', '', '');
}

function gerarTermo( $request ){
	
	global $db;
	
	extract($request);
	
	$outro = false;
	$sql = '';
	if( is_array( $motid ) ){
		$sql = '';
		foreach( $motid as $mot ){
			if( $mot == 'ou' ){
				$outro = true;
			}else{
				if( is_array( $entid[$mot] ) ){
					$arr = $entid[$mot];
					$sql .= "DELETE FROM pse.motivojus WHERE jusid = $jusid AND motid = $mot;
							";
					foreach( $arr as $ent ){
						$mujquantedureduzido[$mot][$ent] = $mujquantedureduzido[$mot][$ent] ? $mujquantedureduzido[$mot][$ent] : 'null';
						$sql .= "INSERT INTO pse.motivojus(jusid,motid,entid,mujtotaleduescola,mujtotaledupactuado,mujquantedureduzido) 
								 VALUES($jusid,$mot,$ent,".$mujtotaleduescola[$mot][$ent].",".$mujtotaledupactuado[$mot][$ent].",".$mujquantedureduzido[$mot][$ent].");
								";
					}
				}
			}
		}
	}
	if( $outro ){
		$jusoutromotivo = substr($jusoutromotivo, 0, 2500);
		$txOutro = "jusoutromotivo = '$jusoutromotivo',"; 
	}
	$sql .= "UPDATE pse.justificativa SET 
				$txOutro
				jusstatusgeratermo = 'S'
			WHERE
				jusid = $jusid;";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('TERMO DE JUSTIFICATIVA DO NÃO ALCANCE DE META FOI GERADO COM SUCESSO !');
			window.location.href = window.location.href;
		  </script>";
}

function recuperaEscolasJusid( $jusid, $motid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				ent.entid,
				ent.entnome,
				coalesce(mujtotaleduescola,coalesce(pamquantedutotal,pacquantedutotal)) as escola,
				coalesce(mujtotaledupactuado,coalesce(pamquanteduseratend,pacquanteduseratend)) as pactuados,
				mujquantedureduzido as reducao
			FROM
				pse.justificativa jus 
			INNER JOIN pse.motivojus muj ON muj.jusid = jus.jusid
			INNER JOIN entidade.entidade ent ON ent.entid = muj.entid
			LEFT  JOIN pse.planoacaoc1 pam ON pam.entid = ent.entid
			LEFT  JOIN pse.planoacaoc2 pac ON pac.entid = ent.entid
			WHERE
				jus.jusid = $jusid AND muj.motid = $motid
			ORDER BY 
				1";
	
	return $db->carregar($sql);
}

function recuperaEscolasConid( $conid, $motid ){
	
	global $db;
	
	$tpcid = '';
	if( $motid == 3 ){
		$tpcid = ' AND tpcid = 1 ';
	}
	
	$sql = "SELECT DISTINCT
				entid,
				entnome,
				max(escola) as escola,
				max(pactuados) as pactuados
			FROM
			(
			(SELECT DISTINCT
				ent1.entid as entid,
				ent1.entnome as entnome,
				pamquantedutotal as escola,
				pamquanteduseratend as pactuados
			FROM
				pse.contratualizacao con 
			INNER JOIN pse.planoacaoc1 pam ON pam.conid = con.conid 
			INNER JOIN entidade.entidade ent1 ON ent1.entid = pam.entid 
			WHERE
				con.conid = $conid $tpcid
			ORDER BY 
				1)
			UNION ALL
			(SELECT DISTINCT
				ent2.entid as entid,
				ent2.entnome as entnome,
				pacquantedutotal as escola,
				pacquanteduseratend as pactuados
			FROM
				pse.contratualizacao con 
			INNER JOIN pse.planoacaoc2 pac ON pac.conid = con.conid 
			INNER JOIN entidade.entidade ent2 ON ent2.entid = pac.entid 
			WHERE
				con.conid = $conid  $tpcid
			ORDER BY 
				1)
			) as foo
			GROUP BY
				entid,
				entnome
			ORDER BY 2";
	
	return $db->carregar($sql);
}

function salvarDadosJustificativa( $request ){
	
	global $db;

	extract($request);
	
	if( $jusid != '' ){
		$sql = "UPDATE pse.justificativa SET
					jussscnpj = '".str_replace(Array('.','/','-'),'',$jussscnpj)."',
					jusssnomesecretario = '$jusssnomesecretario', 
					jusssestadocivil = '$jusssestadocivil', 
					jusssidentidade = '$jusssidentidade', 
					jusssorgaoexp = '$jusssorgaoexp', 
					jussscpf = '".str_replace(Array('.','/','-'),'',$jussscpf)."', 
					jussecnpj = '".str_replace(Array('.','/','-'),'',$jussecnpj)."', 
					jussenomesecretario = '$jussenomesecretario', 
					jusseestadocivil = '$jusseestadocivil', 
					jusseidentidade = '$jusseidentidade', 
					jusseorgaoexp = '$jusseorgaoexp', 
					jussecpf = '".str_replace(Array('.','/','-'),'',$jussecpf)."'
				WHERE
					jusid = $jusid";
		$db->executar($sql);
	}else{
		$sql = "INSERT INTO pse.justificativa(
		            conid, jussscnpj, jusssnomesecretario,
		            jusssestadocivil, jusssidentidade, jusssorgaoexp, 
		            jussscpf, jussecnpj, jussenomesecretario, 
		            jusseestadocivil, jusseidentidade, jusseorgaoexp, 
		            jussecpf)
			    VALUES (
			    	$conid, '".str_replace(Array('.','/','-'),'',$jussscnpj)."', '$jusssnomesecretario', 
			    	'$jusssestadocivil', '$jusssidentidade', '$jusssorgaoexp', 
			    	'".str_replace(Array('.','/','-'),'',$jussscpf)."', '".str_replace(Array('.','/','-'),'',$jussecnpj)."', '$jussenomesecretario', 
			        '$jusseestadocivil', '$jusseidentidade', '$jusseorgaoexp', '".str_replace(Array('.','/','-'),'',$jussecpf)."')
				RETURNING
					jusid;";
		echo $db->pegaUm($sql);
	}
	$db->commit();
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

$cpf = $_SESSION['usucpf'];
$perfis = pegaPerfilGeral();
if( $db->testa_superuser() || !in_array(SECRETARIA_MUNICIPAL,$perfis) || !in_array(SECRETARIA_ESTADUAL,$perfis)  ){
	if( $_REQUEST['funid'] && $_REQUEST['estuf'] && $_REQUEST['muncod'] ){
		$funid = $_REQUEST['funid'];
		if( $funid == 6 ){
			$filtroEnde = "ende.estuf = '".$_REQUEST['estuf']."'";
		}else{
			$filtroEnde = "ende.muncod = '".$_REQUEST['muncod']."'";
		}
		$sql = "SELECT
					ent.entid as entid,
					est.estdescricao as estado,
					mun.mundescricao as municipio,
					entnome as secretaria
				FROM
					pse.usuarioresponsabilidade urs
				INNER JOIN entidade.endereco ende ON ende.estuf = urs.estuf
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN territorios.estado est ON est.estuf = ende.estuf
				INNER JOIN entidade.entidade ent ON ent.entid = ende.entid
				INNER JOIN entidade.funcaoentidade fun ON fun.entid = ent.entid
				WHERE
					$filtroEnde
					AND fun.funid = $funid";
	}else{
		include  APPRAIZ . 'includes/cabecalho.inc';
		echo '<br />';
		//$db->cria_aba( $abacod_tela, $url, '' );
		
		$menu = carregarMenuAbasPse();
		echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
		
		$titulo = "Justificativa Para o Não Alcance de Metas";
		monta_titulo( $titulo, $portaria );
?>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>
		$(document).ready(function(){
		
			$('#aguarde').hide();

			$('.acessar').click(function(){
				var erro = true;
				$('[name="funid"]').each(function(){
					if( $(this).attr('checked') ){
						erro = false;
					}
				});
				$('.obrigatorio').each(function(){
					if( $(this).val() == '' ){
						erro = true;
					}
				});
				if( erro ){
					alert('Preencha todos os campos.');
					return false;
				}else{
					$('#formSuper').submit();
				}
			});

			$('#estuf').change(function(){
				var estuf = $(this).val();
				$.ajax({
					type: "POST",
					url: window.location.href,
					data: "req=atualizaComboMunicipio&estuf="+estuf,
					async: false,
					success: function(msg){
						$('#td_muncod').html(msg);
					}
				});
			});
		});
		</script>
		<form id="formSuper" name="formSuper" method="post" action="">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="">
				<tr>
					<td class="SubTituloDireita" width="15%">UF</td>
					<td>
					<?php 
					$sql = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado
							ORDER BY
								2";
					$db->monta_combo('estuf',$sql, 'S','Selecione...','','','',200,'S', 'estuf', '', $dados['estuf'], '');
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="15%">Município</td>
					<td id="td_muncod">
					<?php 
					if( $dados['estuf'] != '' ){
						$sql = "SELECT
									muncod as codigo,
									mundescricao as descricao
								FROM
									territorios.municipio
								WHERE 
									estuf ilike '".$dados['estuf']."'
								ORDER BY
									2";
					}else{
						$sql = "SELECT
									'' as codigo,
									'Selecione uma UF' as descricao";
					}
					$db->monta_combo('muncod',$sql, 'S','Selecione...','','','',200,'S', 'estuf', '', $dados['muncod'], '');
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top">Esfera:</td>
					<td id="td_municipio">
						<input type="radio" name="funid" value="6"/> Estadual <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"><br>
						<input type="radio" name="funid" value="7"/> Municipal <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="button" value="Acessar Termo" class="acessar"/>
					</td>
				</tr>
			</table>
		</form>
<?php 
		exit;
	}
}elseif( in_array(SECRETARIA_ESTADUAL,$perfis) ){
	$funid = 6;
	$sql = "SELECT
				ent.entid as entid,
				est.estdescricao as estado,
				mun.mundescricao as municipio,
				entnome as secretaria
			FROM
				pse.usuarioresponsabilidade urs
			INNER JOIN entidade.endereco ende ON ende.estuf = urs.estuf
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN territorios.estado est ON est.estuf = ende.estuf
			INNER JOIN entidade.entidade ent ON ent.entid = ende.entid
			INNER JOIN entidade.funcaoentidade fun ON fun.entid = ent.entid
			WHERE
				usucpf = '$cpf'
				AND fun.funid = $funid";
}elseif( in_array(SECRETARIA_MUNICIPAL,$perfis) ){
	$funid = 7;
	$sql = "SELECT
				ent.entid as entid,
				est.estdescricao as estado,
				mun.mundescricao as municipio,
				entnome as secretaria
			FROM
				pse.usuarioresponsabilidade urs
			INNER JOIN entidade.endereco ende ON ende.muncod = urs.muncod
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN territorios.estado est ON est.estuf = ende.estuf
			INNER JOIN entidade.entidade ent ON ent.entid = ende.entid
			INNER JOIN entidade.funcaoentidade fun ON fun.entid = ent.entid
			WHERE
				usucpf = '$cpf'
				AND fun.funid = $funid";
}else{
	echo "<script>
			alert('Acesso Negado.');
			window.location.href = 'pse.php?modulo=inicio&acao=C';
		  </script>";
}

if( date('Ymd') < PRAZO_JUSTIFICATICA_META_INICIO ){
	echo "<script>
			alert('Início para justificativa de não cumprimento de meta dia 01/12/2012.');
			window.location.href = 'pse.php?modulo=inicio&acao=C';
		  </script>";
}

$secretaria = $db->pegaLinha($sql);

if( $secretaria['entid'] == '' ){
	echo "<script>
			alert('Secretaria não encontrada.');
			window.location.href = 'pse.php?modulo=principal/justificativaMetas&acao=A';
		</script>";
	die();
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
//$db->cria_aba( $abacod_tela, $url, '' );

$menu = carregarMenuAbasPse();
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

$titulo = "Justificativa para o não Alcance de Metas";
monta_titulo( $titulo, $portaria );
?>
<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top" width="15%"><b>UF:</b></td>
		<td><?=$secretaria['estado'] ?> </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio"><?=$secretaria['municipio'] ?> </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio"><?=$secretaria['secretaria'] ?> </td>
	</tr>
</table>
<?php 
monta_titulo('', '<div style="font-size:12;"><b>Informações Cadastrais das Secretarias Municipais da Saúde e Educação</b></div>');

$sql = "SELECT
			conid,
			-- educacao
			consecnpj as secnpj,
			consenomesecretario as senomesecretario,
			conseestadocivil as seestadocivil,
			conseidentidade as seidentidade,
			conseorgaoexp as seorgaoexp,
			consecpf as secpf,
			--saude
			consscnpj as sscnpj,
			conssnomesecretario as ssnomesecretario,
			conssestadocivil as ssestadocivil,
			conssidentidade as ssidentidade,
			conssorgaoexp as ssorgaoexp,
			consscpf as sscpf
		FROM
			pse.contratualizacao con
		WHERE
			con.entid = ".$secretaria['entid'];
$info = $db->pegaLinha($sql);

if( $info['conid'] == '' ){
	if( $db->testa_superuser() ){
		echo "<script>
				alert('Essa Secretaria não possui contrato.');
				window.history.back();
			  </script>";
		die();
	}else{
		echo "<script>
				alert('Essa Secretaria não possui contrato.');
				window.location.href = 'pse.php?modulo=inicio&acao=C';
			  </script>";
		die();
	}
}

$sql = "SELECT DISTINCT
			jus.jusid,
			jus.conid,
			jusoutromotivo,
			jusstatusgeratermo,
			-- educacao
			jussecnpj as secnpj,
			jussenomesecretario as senomesecretario,
			jusseestadocivil as seestadocivil,
			jusseidentidade as seidentidade,
			jusseorgaoexp as seorgaoexp,
			jussecpf as secpf,
			--saude
			jussscnpj as sscnpj,
			jusssnomesecretario as ssnomesecretario,
			jusssestadocivil as ssestadocivil,
			jusssidentidade as ssidentidade,
			jusssorgaoexp as ssorgaoexp,
			jussscpf as sscpf,
			--motivos gravados
			mt1.motid as mot1,
			mt2.motid as mot2,
			mt3.motid as mot3,
			jusoutromotivo
		FROM 
			pse.justificativa jus
		LEFT JOIN pse.motivojus mt1 ON mt1.jusid = jus.jusid AND mt1.motid = 1
		LEFT JOIN pse.motivojus mt2 ON mt2.jusid = jus.jusid AND mt2.motid = 2
		LEFT JOIN pse.motivojus mt3 ON mt3.jusid = jus.jusid AND mt3.motid = 3
		WHERE 
			conid = ".$info['conid'];
$infoJust = $db->pegaLinha($sql);

if( $infoJust['jusid'] != '' ){
	$info = $infoJust;
}

$jussecnpj = $info['secnpj'];
$jussenomesecretario = $info['senomesecretario'];
$jusseestadocivil = $info['seestadocivil'];
$jusseidentidade = $info['seidentidade'];
$jusseorgaoexp = $info['seorgaoexp'];
$jussecpf = $info['secpf'];

$jussscnpj = $info['sscnpj'];
$jusssnomesecretario = $info['ssnomesecretario'];
$jusssestadocivil = $info['ssestadocivil'];
$jusssidentidade = $info['ssidentidade'];
$jusssorgaoexp = $info['ssorgaoexp'];
$jussscpf = $info['sscpf'];

$jusoutromotivo = $info['jusoutromotivo'];

$bloq = false;
$habil = 'S';
if( $info['jusstatusgeratermo'] == 'S' ){
	$bloq = true;
	$habil = 'N';
}elseif( !$db->testa_superuser() && !in_array(SECRETARIA_MUNICIPAL,$perfis) && !in_array(SECRETARIA_ESTADUAL,$perfis) ){
	$bloq = true;
	$habil = 'N';
}


if( date('Ymd') > PRAZO_JUSTIFICATICA_META_FIM ){
	echo "<script>
	alert('Prazo para justificativa de não cumprimento de meta encerrado!');
	//window.location.href = 'pse.php?modulo=inicio&acao=C';
	</script>";
	$bloq = true;
	$habil = 'N';
}


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

var mensagem1 = 'TEM CERTEZA QUE OS DADOS INFORMADOS ESTÃO CORRETOS ?';
var mensagem2 = 'APÓS GERADO O TERMO DE JUSTIFICATIVA, OS DADOS NÃO PODERÃO SER ALTERADOS PELO USUÁRIO!';

$(document).ready(function(){

	$('.normal').keyup();

	$('#aguarde').hide();

	if( $('#jusid').val() == '' ){
		$('.bt_gerar_termo').attr('disabled',true);
		$('.bt_gerar_termo').attr('title','Primeiro confirme os dados das Secretarias de Saúde e Educação.');
	}else{
		$('.bt_gerar_termo').attr('disabled',false);
		$('.bt_gerar_termo').attr('title',' ');
	}
	
	$('#bt_confirmar_dados').click(function(){
		var jusid = $('#jusid').val();
		$.ajax({
			type: "POST",
			url: window.location.ref,
			data: "req=salvarDadosJustificativa&jusid="+jusid+"&"+$('#formSecretaria').serialize(),
			async: false,
			success: function(msg){
				if( msg != '' ){
					$('#jusid').val(msg);
				}
				alert('Dados gravados com sucesso.');
				if( $('#jusid').val() == '' ){
					$('.bt_gerar_termo').attr('disabled',true);
					$('.bt_gerar_termo').attr('title','Primeiro confirme os dados das Secretarias de Saúde e Educação.');
				}else{
					$('.bt_gerar_termo').attr('disabled',false);
					$('.bt_gerar_termo').attr('title',' ');
				}
			}
		});
	});

	$('.motivo').click(function(){
		if( $(this).attr('checked') ){
			$('#tr_'+$(this).val()).show();
		}else{
			$('#tr_'+$(this).val()).hide();
		}
	});

	$('.bt_gerar_termo').click(function(){
		var erro = true;
		var motid = '';
		var para = false;
		if( $('.motivo:checked').length > 0 ){
			$('.motivo:checked').each(function(){
				motid = $(this).val();
				if( motid != 'ou' ){
					$('[name="entid['+motid+'][]"]').each(function(){
						if( $(this).attr('checked') ){
							erro = false;
						}
					});
					if( erro ){
						alert('Selecione pelo menos 1(uma) escola para cada motivo selecionado.');
						para = true;
						return false;
					}else{
						erro = true;
					}
				}else{
					if( $('#jusoutromotivo').val() == '' ){
						alert('Preencha a descrição do motivo "Outros".');
						para = true;
						return false;
					}
				}
			});
		}else{
			if( $('#jusoutromotivo').val() == '' || !$('#motidou').attr('checked') ){
				alert('É obrigatória a escolha de pelo menos 1(um) motivo.');
			}
		}
		if( para ){
			return false;
		}
		if( confirm(mensagem1) ){
			if( confirm(mensagem2) ){
				$('#req').val('gerarTermo');
				$('#formMotivos').submit();
			}
		}
	});

	$('.bt_imprimir_termo').click(function(){
		var jusid = $('#jusid').val();
		return windowOpen( '?modulo=principal/popUpImpressaoJustificativaMetas&acao=C&jusid='+jusid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});

});
</script>
<form id="formSecretaria" name="formSecretaria" method="post" action="">
	<input type="hidden" name="conid" value="<?=$info['conid'] ?>"/>
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td bgcolor="#c0c0c0" width="20%"></td>
			<td align="left" bgcolor="#c0c0c0" style="font-size:14;"><b>Secretaria de Saúde</b></td>
			<td align="left" bgcolor="#c0c0c0" style="font-size:14;"><b>Secretaria de Educação</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">C.N.P.J. da Secretaria</td>
			<td><?=campo_texto('jussscnpj','N',$habil,'','20','18','##.###.###/####-##','', '', '','', 'id="jussscnpj"','', $jussscnpj ); ?> </td>
			<td><?=campo_texto('jussecnpj','N',$habil,'','20','18','##.###.###/####-##','', '', '','', 'id="jussecnpj"','', $jussecnpj ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Nome completo do Secretário</td>
			<td><?=campo_texto('jusssnomesecretario','N',$habil,'','70','60','','', '', '','', 'id="jusssnomesecretario"','', $jusssnomesecretario ); ?> </td>
			<td><?=campo_texto('jussenomesecretario','N',$habil,'','70','60','','', '', '','', 'id="jussenomesecretario"','', $jussenomesecretario ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Estado civil do Secretário</td>
			<td><?=campo_texto('jusssestadocivil','N',$habil,'','20','15','','', '', '','', 'id="jusssestadocivil"','', $jusssestadocivil ); ?> </td>
			<td><?=campo_texto('jusseestadocivil','N',$habil,'','20','15','','', '', '','', 'id="jusseestadocivil"','', $jusseestadocivil ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Número da Carteira de Identidade</td>
			<td><?=campo_texto('jusssidentidade','N',$habil,'','20','20','[#]','', '', '','', 'id="jusssidentidade"','', $jusssidentidade ); ?> </td>
			<td><?=campo_texto('jusseidentidade','N',$habil,'','20','20','[#]','', '', '','', 'id="jusseidentidade"','', $jusseidentidade ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Orgão Expedidor</td>
			<td><?=campo_texto('jusssorgaoexp','N',$habil,'','20','11','','', '', '','', 'id="jusssorgaoexp"','', $jusssorgaoexp ); ?> </td>
			<td><?=campo_texto('jusseorgaoexp','N',$habil,'','20','11','','', '', '','', 'id="jusseorgaoexp"','', $jusseorgaoexp ); ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">CPF</td>
			<td><?=campo_texto('jussscpf','N',$habil,'','20','14','###.###.###-##','', '', '','', 'id="jussscpf"','', $jussscpf ); ?> </td>
			<td><?=campo_texto('jussecpf','N',$habil,'','20','14','###.###.###-##','', '', '','', 'id="jussecpf"','', $jussecpf ); ?> </td>
		</tr>
		<tr>
			<td align="left" bgcolor="#c0c0c0" colspan="3" style="color:red">
				<b>Atenção: Se necessário, altere as informações e depois confirme. Se não, basta confirmar.</b>
				<div style="float:right">
					<input type="button" id="bt_confirmar_dados" <?=($bloq ? 'disabled="disabled"' : '' ) ?> value="Confirme os Dados" />
				</div>
			</td>
		</tr>
	</table>
</form>
<form id="formMotivos" name="formMotivos" method="post" action="">
	<input type="hidden" name="req"   id="req"   value=""/>
	<input type="hidden" name="jusid" id="jusid" value="<?=$info['jusid'] ?>"/>
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<tr>
			<td bgcolor="#c0c0c0" colspan="3" style="font-size:12" align="center">
				<b>Indique o(s) motivo(s) para o não alcance de metas do seu município <label style="color:red">(pode marcar mais de um)</label></b>
			</td>
		</tr>
		<?php 
		$sql = "SELECT motid, motdescricao FROM pse.motivo ORDER BY 2";
		$motivos = $db->carregar($sql);
		foreach( $motivos as $motivo ){
		?>
		<tr>
			<td class="SubTituloDireita">
				<div style="float:left;vertical-align:middle;line-height:25px;" >
					<input type="checkbox" name="motid[]" id="motid_<?=$motivo['motid'] ?>" class="motivo" value="<?=$motivo['motid'] ?>" <?=($info['mot'.$motivo['motid']] == $motivo['motid'] ? 'checked="checked"' : '') ?> <?=($bloq ? 'disabled="disabled"' : '' ) ?>/> <?=$motivo['motdescricao'] ?>
				</div>
			</td>
		</tr>
		<tr id="tr_<?=$motivo['motid'] ?>" style="<?=($bloq && $info['mot'.$motivo['motid']] == $motivo['motid'] ? '' : 'display:none' ) ?>">
			<td>
				<div style="overflow:auto;height:150px;background-color:white" >
					<?php 
					if( $bloq && $info['jusid'] ){
						$escolas = recuperaEscolasJusid( $info['jusid'], $motivo['motid'] );
					}else{
						$escolas = recuperaEscolasConid( $info['conid'], $motivo['motid'] );
					}
					if( is_array($escolas) ){
						foreach( $escolas as $escola ){
					?>
					<input type="checkbox" name="entid[<?=$motivo['motid'] ?>][]" value="<?=$escola['entid'] ?>" <?=($bloq ? 'disabled="disabled" checked="checked"' : '' ) ?>/> 
					<?=$escola['entnome'] ?> 
					- <b>Educandos da Escola:</b> <?=$escola['escola'] ?> 
					<input type="hidden" name="mujtotaleduescola[<?=$motivo['motid'] ?>][<?=$escola['entid'] ?>]" value="<?=$escola['escola'] ?>"/>
					
					- <b>Educandos Pactuados:</b> <?=$escola['pactuados'] ?> 
					<input type="hidden" name="mujtotaledupactuado[<?=$motivo['motid'] ?>][<?=$escola['entid'] ?>]" value="<?=$escola['escola'] ?>"/> 
					<?php
							if( $motivo['motid'] == 1 ){
					?>
					- <b>Redução de Educandos:</b>
					<?=campo_texto('mujquantedureduzido['.$motivo['motid'].']['.$escola['entid'] .']','N',$habil,'','20','20','[#]','', '', '','', 'id="mujquantedureduzido"','', $escola['reducao'] ); ?>
					<?php 
							} 	
					?>
					<br>
					<?php 
						}
					}
					?>
				</div>
			</td>
		</tr>
		<?php 
		}
		?>
		<tr>
			<td class="SubTituloDireita">
				<div style="float:left;vertical-align:middle;line-height:25px;" >
					<input type="checkbox" name="motid[]" id="motidou" class="motivo" value="ou" <?=($info['jusoutromotivo'] != '' ? 'checked="checked"' : '') ?> <?=($bloq ? 'disabled="disabled"' : '' ) ?>/> Outros
				</div>
			</td>
		</tr>
		<tr id="tr_ou" style="<?=($info['jusoutromotivo'] != '' ? '' : 'display:none') ?>">
			<td>
				<?=campo_textarea( 'jusoutromotivo', 'N', $habil, '', 200, 5, 2500, '', '', '', '', '', $jusoutromotivo, ''); ?>
			</td>
		</tr>
		<tr>
			<td align="left" bgcolor="#c0c0c0" colspan="3" style="color:red;vertical-align:middle;">
				<b>Atenção: Após gerar o termo de justificativa de não alcance de metas não é possivel alterá-lo.</b>
				<div style="float:right">
					<?php if( $bloq && $info['jusid'] ){?>
					<input type="button" class="bt_imprimir_termo" value="Imprimir" id="<?=$info['jusid'] ?>" />
					<?php }else{?>
					<input type="button" class="bt_gerar_termo" value="Gerar Termo de Justificativa" />
					<?php }?>
				</div>
			</td>
		</tr>
	</table>
</form>