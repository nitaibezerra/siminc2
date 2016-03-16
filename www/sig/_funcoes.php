<?
function removernotatecnica($dados) {
	global $db;
	
	$sql = "SELECT arqid FROM academico.notatecnica WHERE ntcid = '".$dados['ntcid']."'";
	$arqid = $db->pegaUm($sql);
	//deletando nota técnica
	$sql = "DELETE FROM academico.notatecnica WHERE ntcid='".$dados['ntcid']."'";
	$db->executar($sql);
	//deletando pdf em public.arquivo
	if($arqid){
		$sql ="DELETE FROM public.arquivo WHERE arqid = '$arqid'";
		$db->executar($sql);
	}
	$db->commit();
	//deletando o arquivo pdf físico do servidor
	if($arqid){
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if(file_exists($caminho)){
			unlink($caminho);
		}
	}
	echo "<script>
			alert('Nota técnica removida com sucesso');
			window.location = '?modulo=principal/notatecnica&acao=A';
		  </script>";
	
}
	

function removercampuscurso($dados) {
	global $db;
	$sql = "DELETE FROM academico.campuscurso WHERE cmpid='". $dados['cmpid'] ."' AND curid='".$dados['curid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
function inserircampuscurso($dados) {
	global $db, $anosanalisados;
	if($anosanalisados[$dados['orgid']]) {
		$anos = $anosanalisados[$dados['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	foreach($anos as $ano) {
		// Validar para naão deixar o sistema inserir arquivo duplicados
		$cpcid = $db->pegaUm("SELECT cpcid FROM academico.campuscurso WHERE curid='". $dados['curid'] ."' AND cmpid='". $dados['cmpid'] ."' AND cpcano='".$ano."'");
		if(!$cpcid) {		
			$sql = "INSERT INTO academico.campuscurso(
    	   			curid, cmpid, cpcano, cpcqtd)
	 				VALUES ('". $dados['curid'] ."','". $dados['cmpid'] ."','". $ano ."',NULL);";
			echo $sql."<br />";
			$db->executar($sql);
		}
	}
	$db->commit();
	exit;
}

function listarcursos($dados) {
	global $db;
	$tpcpossuientidade = $db->pegaUm("SELECT tpcpossuientidade FROM public.tipocurso WHERE tpcid='". $dados['tpcid'] ."'");
	if($tpcpossuientidade == "t") {
		$entid = $db->pegaUm("SELECT entid FROM academico.campus WHERE cmpid='". $dados['cmpid'] ."'");
		$sql = "SELECT '<input type=\"checkbox\" value=\"'|| curid ||'\" onclick=\"crtcursos(this);\" '|| CASE WHEN (SELECT cc1.curid FROM academico.campuscurso cc1 WHERE cc1.curid = cc2.curid AND cc1.cmpid ='".$dados['cmpid']."' GROUP BY cc1.curid) IS NULL THEN '' ELSE 'checked' END ||'>' as codigo, curdsc as curso, turdsc FROM public.curso cc2 LEFT JOIN public.turno tur ON tur.turid = cc2.turid WHERE tpcid = '". $dados['tpcid'] ."' AND entid='". $entid ."' ORDER BY curdsc";		
		$cabecalho = array("","Cursos", "Turno");
	} else {
		$sql = "SELECT '<input type=\"checkbox\" value=\"'|| curid ||'\" onclick=\"crtcursos(this);\" '|| CASE WHEN (SELECT cc1.curid FROM academico.campuscurso cc1 WHERE cc1.curid = cc2.curid AND cc1.cmpid ='".$dados['cmpid']."' GROUP BY cc1.curid) IS NULL THEN '' ELSE 'checked' END ||'>' as codigo, curdsc as curso FROM public.curso cc2 WHERE tpcid = '". $dados['tpcid'] ."' ORDER BY curdsc";
		$cabecalho = array("","Cursos");
	}

	$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '100%','N');
	exit;
}

function listarMunicipioAjax($dados) {
	global $db;
	$cmpid = $dados['cmpid']; 	
	$sql = sprintf("SELECT 
					en.estuf AS uf,
					m.mundescricao AS municipio
					FROM 
					 entidade.entidade e
					INNER JOIN entidade.endereco en ON( e.entid = en.entid AND en.tpeid = 1)
					INNER JOIN territorios.municipio m on m.muncod = en.muncod 
					WHERE e.entid = %d
					ORDER BY 
					 municipio", $cmpid);	
 	
	$dados = $db->pegaLinha($sql);	
	$saida = $dados['uf']."|".$dados['municipio'];	
	echo $saida;
	exit;
}
function listarCampusAjax($dados) {
	global $db; 	
 	$entid_unidade = $dados['entid_unidade']; 
	$sql = sprintf("SELECT entid AS codigo, entnome AS descricao			
					FROM entidade.entidade ent 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid				
					WHERE fea.entid = %d and entid NOT IN(SELECT entid FROM academico.campus)
					ORDER BY descricao", $entid_unidade); 	
	
	$dados = $db->carregar($sql);
	$saida = '';
	$saida .= "<option value=''>Selecione</option>";
	foreach ($dados as $dado) {
		$saida .= "<option value='". $dado['codigo']."' ".$sel.">". $dado['descricao'] ."</option>";		
	}
	$saida .= obrigatorio();	
	echo $saida;
	exit;
}
function listarUnidadesAjax($dados) {
	global $db; 	
 	$orgid = $dados['orgid'];
	if($orgid == 1) $funid = 12;
	else if($orgid == 2) $funid = 11;
 	$sql = sprintf("SELECT entid AS codigo, entnome AS descricao			
					FROM entidade.entidade 					
					WHERE funid = %d
					ORDER BY descricao", $funid); 	
	
	$dadosent = $db->carregar($sql);
	$saida = '';
	$saida .= "<option value=''>Selecione</option>";
	foreach ($dadosent as $dado) {
		$saida .= "<option value='". $dado['codigo']."' ".$sel.">". $dado['descricao'] ."</option>";		
	}	
	$saida .= obrigatorio();
	echo $saida;
	exit;	
}

function carregarlistaobras($dados) {
	global $db;
	
	if($_SESSION['sig_var']['iscampus'] == 'sim') {
		$filtro = "cam.entid='".$_SESSION['sig_var']['entid']."'"; 
	} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
		$filtro = "au.entid='".$_SESSION['sig_var']['entid']."'";
	} else {
		echo "Problemas no carregamento da lista. Entre em contato com a equipe técnica.";
		exit;
	}
	$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array("filtro" => "cnt.obrid=".$uni['obrid']." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
	if($dados['buscar']=='serinaugurada') {
		$filtro .= " AND obi.obcid IS NOT NULL";
		$complemento = "(CASE WHEN 
						(SELECT arq.arqid FROM public.arquivo arq
						 INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						 WHERE oar.obrid=obr.obrid AND aqostatus = 'A' AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR	arqtipo = 'image/png') LIMIT 1) IS NULL 
						 THEN '' 
						 ELSE '<center><img title=\"Clique para visualizar\" src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"javascript:window.open(\'../slideshow/slideshow/ajustarimgparam2.php?_sisarquivo=obras&obrid='||obr.obrid||'\',\'imagem\',\'width=850,height=600,resizable=yes\');\"></center>' 
						 END) as complemento, ";
		
	} elseif($dados['buscar']=='outras') {
		$filtro .= " AND obi.obcid IS NULL";
		$complemento = "(CASE WHEN 
						(SELECT arq.arqid FROM public.arquivo arq
						 INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						 WHERE oar.obrid=obr.obrid AND aqostatus = 'A' AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR	arqtipo = 'image/png') LIMIT 1) IS NULL 
						 THEN '' 
						 ELSE '<center><img title=\"Clique para visualizar\" src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"javascript:window.open(\'../slideshow/slideshow/ajustarimgparam2.php?_sisarquivo=obras&obrid='||obr.obrid||'\',\'imagem\',\'width=850,height=600,resizable=yes\');\"></center>' 
						 END) as complemento, ";
	} else {
		// Listar todos com o checkbox
		$complemento = "'<input type=\"checkbox\" onclick=\"crtobrasinauguradas(this,\''||cam.cmpid||';'||obr.obrid||'\');\" value=\"'||cam.cmpid||'¨¨¨'||obr.obrid||'\" '|| CASE WHEN (SELECT obcid FROM academico.obrainauguradacampus WHERE obrid=obr.obrid AND cmpid=cam.cmpid) IS NULL THEN '' ELSE 'checked' END ||'>' as complemento,";
	}
	$sql = "SELECT ".$complemento." e.entnome as campus, obr.obrdesc, mun.mundescricao||'/'||en.estuf as local, sto.stodesc, obr.obrid FROM academico.campus cam 
			LEFT JOIN obras.obrainfraestrutura obr ON obr.entidcampus = cam.entid  
			LEFT JOIN entidade.entidade e ON e.entid = obr.entidcampus 
			LEFT JOIN entidade.endereco en ON en.endid = obr.endid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod 
			LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
			LEFT JOIN academico.obrainauguradacampus obi ON obr.obrid = obi.obrid
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
			LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
			LEFT JOIN entidade.entidade au ON fea.entid = au.entid 
			WHERE ".$filtro." AND obr.obsstatus='A' AND (obr.obrstatusinauguracao ='N' OR obr.obrstatusinauguracao IS NULL) ORDER BY au.entnome, e.entnome";
	$registros = $db->carregar($sql);
	echo "<table width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\">";
	if($registros[0]) {
		echo "<thead><tr>";
		echo "<td align=\"center\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">&nbsp;</td>";
		echo "<td align=\"center\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">Campus / Uned</td>";
		echo "<td align=\"center\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">Nome da obra</td>";
		echo "<td align=\"center\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">Munícipio / UF</td>";
		echo "<td align=\"center\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">Situação da obra</td>";
		echo "</tr></thead>";
    	echo "<tbody>";
		for ($i=0;$i<count($registros);$i++) {
			$_SESSION['imgparametos'][$registros[$i]["obrid"]] = array("filtro" => "cnt.obrid=".$registros[$i]["obrid"]." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			echo "<tr bgcolor=\"".$marcado."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$marcado."';\">";
			echo "<td>".$registros[$i]['complemento']."</td>";
			echo "<td title=\"Campus / Uned\">".$registros[$i]['campus']."</td>";
			echo "<td title=\"Nome da obra\">".$registros[$i]['obrdesc']."</td>";
			echo "<td title=\"Munícipio / UF\">".$registros[$i]['local']."</td>";
			echo "<td title=\"Situação da obra\">".$registros[$i]['stodesc']."</td>";
			echo "</tr>";
		}
    	echo "</tbody>";
	} else {
    	echo "<tr><td align=\"center\" style=\"color:#cc0000;\">Não foram encontrados Registros.</td></tr>";
	}
    echo "</table>";
	exit;
}

function carregarvagasporcurso($dados) {
	global $db,$anosanalisados;

	// pegando dados sobre o tipo de curso, verifica apenas o primeiro registro
	// se for por entidade (Ensino Superior) insere o turno
	// se por acaso colocarem vários tipos sendo com o campo tpc.tpcpossuientidade diferentes,
	// modificar a forma de distinção neste código. Solicitação feita pelo analista: Hugo Morais
	$sql = "SELECT tpc.tpcpossuientidade FROM academico.campuscurso cmc2 
			LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
			LEFT JOIN public.turno tur ON tur.turid = cur.turid 
			LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
			WHERE cmc2.cmpid='".$dados['cmpid']."' GROUP BY tpc.tpcpossuientidade LIMIT 1";
	$tipocurso = $db->pegaUm($sql);
	
	$cabecalho[] = "";
	$cabecalho[] = "Tipo de curso";
	$cabecalho[] = "Cursos";
	
	// verifica se é por entidade, logo necessita do turno
	if($tipocurso == "t") {
		$cabecalho[] = "Turno";
	}
	
	if($anosanalisados[$dados['orgid']]) {
		$anos = $anosanalisados[$dados['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	// Pegando mascara definida para a quantidade de cursos (constantes.php)
	$mask = $db->carregar("SELECT tpimascara,tpitamanhomax FROM academico.tipoitem WHERE tpiid='".TIPOITEM_QTD."'");
	if($mask) {
		$mask = current($mask);
	}
	foreach($anos as $ano) {
		$cabecalho[] = $ano;
		$inputs[] = "'<input ".(($mask['tpitamanhomax'])?"maxlength=\"".$mask['tpitamanhomax']."\"":"")." ".(($mask['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal(\'" . $mask['tpimascara'] . "\',this.value);calculacoluna(this);\"":"")." type=\"text\" name=\"cursos['|| cmc2.curid ||'][".$ano."]\" size=\"12\" class=\"normal\" value=\"'|| (SELECT coalesce(cast(cpcqtd as varchar),'') FROM academico.campuscurso cmc1 WHERE cmc1.curid = cmc2.curid AND cmc1.cpcano='".$ano."' AND cmc1.cmpid='".$dados['cmpid']."') ||'\">' as ano".$ano;
		$totalizador[$ano] = "<input type='text' size='12' class='normal' name='tot".$ano."' value='' readonly>";
	}
//	'<img src=\"../imagens/excluir_01.gif\" onclick=\"removercurso('|| cmc2.curid ||')\">' as acao
	// verifica se é por entidade, logo necessita do turno. Faz analise na construção do SELECT
	$sql = "SELECT '<img src=\"../imagens/excluir_01.gif\" onclick=\"alert(\'Permissão negada!\');\">' as acao, tpc.tpcdsc, cur.curdsc as curso, ". (($tipocurso=="t")?"tur.turdsc,":"") ." ".implode(",",$inputs)." FROM academico.campuscurso cmc2 
			LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
			". (($tipocurso=="t")?"LEFT JOIN public.turno tur ON tur.turid = cur.turid":"") ."
			LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
			WHERE cmc2.cmpid='".$dados['cmpid']."' GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc ".(($tipocurso=="t")?", tur.turdsc":"");
	// Quase identica ao monta lista simples, porém adicionei uma última linha com os contadores em javascript
	$RS = $db->carregar($sql);
	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)	{
		//Monta Cabeçalho
		if(is_array($cabecalho)) {
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
			}
			print '</tr> </thead>';
		}

        echo '<tbody>';
		//Monta Listagem
		$totais = array();
		$tipovl = array();
		for ($i=0;$i<$nlinhas;$i++) {
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {
				print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			print '</tr>';
		}
		print '<tr>';
		if(is_array($cabecalho)) {
			print '<td class="title">&nbsp;</label>';
			print '<td class="title">&nbsp;</label>';
			// 	verifica se é por entidade, logo necessita do turno.
			if($tipocurso == "t") {
				print '<td class="title">&nbsp;</label>';
			}
			print '<td class="title" align=\'right\'><strong>TOTAL:</strong></label>';
			foreach($cabecalho as $campo) {
				if($totalizador[$campo]) {
					print '<td class="title">'.$totalizador[$campo].'</label>';
				}
			}
		}
		print '</tr>';
        print '</tbody>';
	}
	print '</table>';
	exit;
}
function ordenaritens($dados) {
	global $db;
	$sql = "SELECT tei.teiordem FROM academico.orgaoitem tei WHERE tei.itmid = '". $dados['itematual'] ."' AND tei.orgid = '". $dados['orgid'] ."'";
	$ordematual = $db->pegaUm($sql);
	$sql = "SELECT tei.teiordem FROM academico.orgaoitem tei WHERE tei.itmid = '". $dados['itemir'] ."' AND tei.orgid = '". $dados['orgid'] ."'";
	$ordemir = $db->pegaUm($sql);
	if($ordemir) {
		$sql = "UPDATE academico.orgaoitem SET teiordem = '". $ordemir ."' WHERE itmid = '". $dados['itematual'] ."' AND orgid = '". $dados['orgid'] ."'";
		$db->executar($sql);
	}
	if($ordematual) {
		$sql = "UPDATE academico.orgaoitem SET teiordem = '". $ordematual ."' WHERE itmid = '". $dados['itemir'] ."' AND orgid = '". $dados['orgid'] ."'";
		$db->executar($sql);
	}
	$db->commit();
	exit;
}

function listaitens($dados) {
	global $db;
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cadastraritens&acao=E&itmid=' || itm.itmid || '\'\");\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/cadastraritens&acao=R&alterabd=R&orgid=' || tei.orgid || '&itmid=' || itm.itmid || '\',\'Deseja realmente excluir este item?\');\">' AS acao, itmdsc, itm.itmid, itm.itmobs, itm.itmglobal 
			FROM academico.item itm 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". (($dados['orgid'])?$dados['orgid']:TIPOENSINO_DEFAULT) ."' AND itm.itmglobal = false
			ORDER BY tei.teiordem";
	$dadositens = $db->carregar($sql);
			
	?>
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
	<thead>
	<tr>
		<td colspan="4" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Itens por ano</strong></td>
	</tr>
	<tr>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ações</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Item</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ordem</td>
	</tr>
	</thead>
	<tbody>
	<?
	if($dadositens) { 
		$i = 0;
		foreach($dadositens as $item) {
			unset($setas);
			if($i == 0) {
				$setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i+1)]['itmid'].");' border='0' title='Descer'>"; 
			} elseif($i == (count($dadositens)-1)) {
				$setas = "<img src='/imagens/seta_cima.gif' border='0' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i-1)]['itmid'].");' style='cursor:pointer;' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";						
			} elseif(count($dadositens) === 1) {
				$setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
			} else {
				$setas = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i-1)]['itmid'].");' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i+1)]['itmid'].");' border='0' title='Descer'>";
			}
		?><tr<? echo (($i%2)?'bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"':'bgcolor="#F7F7F7" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#F7F7F7\'"');?>>
		  	<td title="Ações"><? echo $item['acao']; ?></td>
			<td title="<?=$item['itmobs'] ?>"><? echo $item['itmdsc'] ?></td>
			<td title="Ordem" align="center"><? echo $setas; ?></td>
		  </tr>
		<? $i++;
		}
	} else {
		?><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr><?
	}
	
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cadastraritens&acao=E&itmid=' || itm.itmid || '\'\");\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/cadastraritens&acao=R&alterabd=R&orgid=' || tei.orgid || '&itmid=' || itm.itmid || '\',\'Deseja realmente excluir este item?\');\">' AS acao, itmdsc, itm.itmid, itm.itmobs, itm.itmglobal 
			FROM academico.item itm 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". (($dados['orgid'])?$dados['orgid']:TIPOENSINO_DEFAULT) ."' AND itm.itmglobal = true
			ORDER BY tei.teiordem";
	$dadositens = $db->carregar($sql);
			
	?>
	<thead>
	<tr>
		<td colspan="4" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Itens globais</strong></td>
	</tr>
	<tr>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ações</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Item</td>
		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ordem</td>
	</tr>
	</thead>
	<tbody>
	<?
	if($dadositens) { 
		$i = 0;
		foreach($dadositens as $item) {
			unset($setas);
			if($i == 0) {
				$setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i+1)]['itmid'].");' border='0' title='Descer'>"; 
			} elseif($i == (count($dadositens)-1)) {
				$setas = "<img src='/imagens/seta_cima.gif' border='0' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i-1)]['itmid'].");' style='cursor:pointer;' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";						
			} elseif(count($dadositens) === 1) {
				$setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
			} else {
				$setas = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i-1)]['itmid'].");' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(". $item['itmid'] .",".$dadositens[($i+1)]['itmid'].");' border='0' title='Descer'>";
			}
		?><tr<? echo (($i%2)?'bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"':'bgcolor="#F7F7F7" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#F7F7F7\'"');?>>
		  	<td title="Ações"><? echo $item['acao']; ?></td>
			<td title="<?=$item['itmobs'] ?>"><? echo $item['itmdsc'] ?></td>
			<td title="Ordem" align="center"><? echo $setas; ?></td>
		  </tr>
		<? $i++;
		}
	} else {
		?><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr><?
	}
	?>
	</table>
	<?
	exit;
}
function carregaitens($dados) {
	global $db,$anosanalisados;
	// em fase de teste... se nao der erro, apagar...
	echo "<pre>";
	print_r($dados);
	exit;
	$cabecalho[] = "Itens";
	if($anosanalisados) {
		foreach($anosanalisados as $ano) {
			$paramselects[] = "'<input class=\"normal\" id=\"' || itm.itmid || '".$ano."\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || '][". $ano ."]\" '|| CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"10\" type=\"text\" value=\"' || CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dados['cmpid'] ."') is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dados['cmpid'] ."') END || '\"> '|| CASE WHEN itm.itmpermiteobs IS TRUE THEN '<img src=\"../imagens/principal.gif\" '|| CASE WHEN (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') IS NULL THEN 'border=\"0\"' ELSE 'border=\"1\" style=\"border-color: red;\"' END ||' id=\"img' || itm.itmid || '_". $ano ."\" onclick=\"abreobservacao(\''|| itm.itmid ||'_".$ano."\');\"><input type=\"hidden\" value=\"' || CASE WHEN (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dados['cmpid'] ."') IS NULL THEN '' ELSE (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dados['cmpid'] ."') END || '\" id=\"'|| itm.itmid ||'_".$ano."\" name=\"obs['|| itm.itmid ||'][".$ano."]\">' ELSE '' END  ||'' AS ano_".$ano;
			$cabecalho[] = $ano;		
		}
		$paramselects = implode(",",$paramselects);
	}
	$sql = "SELECT '<strong>'||itm.itmdsc||'</strong>',
			". $paramselects ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $dados['orgid'] ."'
			ORDER BY tei.teiordem";
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '95%','N');
	exit;
}
function carregacampus($dados) {
	global $db, $anosanalisados, $_funcoesentidade;
	
	$permissoes = verificaPerfilSig();
			
	if($permissoes['remover']) {
		$excluircampus = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=inicio&acao=E&cmpid=' || cam.cmpid || '\',\'Deseja realmente excluir este campus?\');\">";
	}
	if($dados['unidade']) {
		$unidade = $dados['unidade'];			
			
 		$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_REQUEST['orgid']."&cmpid=' || cam.cmpid || '\';\"> ".$excluircampus."' as acao,
    			 '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&orgid=".$_REQUEST['orgid']."&cmpid=' || cam.cmpid || '\';\">'||e.entnome||'</a>' as campus, 
		    	 en.estuf AS uf,
				 m.mundescricao AS municipio 
				FROM entidade.entidade e 
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				INNER JOIN entidade.entidade au ON fea.entid = au.entid
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
				INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid
				INNER JOIN academico.campus cam on e.entid = cam.entid					
				LEFT JOIN entidade.endereco en ON en.entid = e.entid 
				LEFT JOIN territorios.municipio m on m.muncod = en.muncod  
				WHERE fea.entid = '". $unidade ."' AND fen2.funid='".$_funcoesentidade[$dados['orgid']]['unidade']."' AND fen.funid='".$_funcoesentidade[$dados['orgid']]['campus']."' ORDER BY e.entnome";
	
		$cabecalho = array( "Ação", "Nome do campus/unidade", "UF", "Município");
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%');
	}
	exit;
}

function processaInsercaoCampusItem($dados = array()) {
	/* Montando um array com os indices das variaveis
	 * para identificar qual o indice contem contem as váriaiveis para serem salvas 
	 */ 
	$indicestodos = array_keys($_POST);
	// Verifica se existe itens no array
	if($indicestodos) {
		// Varrendo os indices
		
		foreach($indicestodos as $ind) {
			// Verifica se o indice tem o termo "gravacampo_"
			$iscampogravacao = strpos($ind,'gravacaocampo_');
			// Se tiver, executar o procedimento de gravação 
			if($iscampogravacao !== false) {
				if($_REQUEST[$ind]) {
					// Campo na qual sera gravado os itens da tabela "campusitem"
					$campogravacao = str_replace("gravacaocampo_","",$ind);
					$campogravacao = explode("_",$campogravacao);
					
					foreach($_REQUEST[$ind] as $itmid => $valor) {						
						if($valor && (count($valor) > 1)) {
							foreach($valor as $ano => $campo) {
								if($campo !== "" || $_POST['obs'][$itmid][$ano]) {
									switch($campogravacao[1]) {
										case 'MONEY':
											$campo = str_replace(array(".",","),array("","."),$campo);
											break;
									}
									$sql[] = "INSERT INTO academico.campusitem(
	   		        						cmpid, itmid, ".$campogravacao[0].",cpidata, cpiano, cpiobs)
					    					VALUES ('". $_POST['cmpid'] ."', '". $itmid ."', ". (($campo !== "")?"'".$campo."'":"NULL") .", '". date("Y-m-d") ."', '". $ano ."',  '". $_POST['obs'][$itmid][$ano] ."');";
								}
							}
						}
						// gravando os dados globais (não possui vinculo com ano)
						if($valor !== "" && (count($valor) == 1)) {
									switch($campogravacao[1]) {
										case 'MONEY':
											$valor = str_replace(array(".",","),array("","."),$valor);
											break;
									}
									$sql[] = "INSERT INTO academico.campusitem(
	   		        						cmpid, itmid, ".$campogravacao[0].",cpidata, cpiobs, cpiano)
					    					VALUES ('". $_POST['cmpid'] ."', '". $itmid ."', ". (($valor !== "")?"'".$valor."'":"NULL") .", '". date("Y-m-d") ."',  '". $_POST['obs'][$itmid] ."', '".date("Y")."');";
								
						}
					}
				}
			}
		}
	}
	return ($sql)?$sql:false;
}


function verificaPerfilSig() {
	global $db;

	$sql = "SELECT 
				p.pflcod 
			FROM 
				seguranca.perfil p 
			LEFT JOIN 
				seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE 
				pu.usucpf = '". $_SESSION['usucpf'] ."' 
				and p.pflstatus = 'A' 
				and p.sisid =  '". SISID ."'";
	$perfilid = $db->pegaUm($sql);
	
	if($db->testa_superuser() || $perfilid == PERFIL_ADMINISTRADOR) {
		// Selecionando tipos de ensino (TODOS)
		$sql = "SELECT 
					orgid 
				FROM 
					academico.orgao";
		$orgids = (array) $db->carregar($sql);
		foreach($orgids as $tpe) {
			$permissoes['vertipoensino'][] = $tpe['orgid'];
		}
		$permissoes['remover'] = true;
		$permissoes['gravar'] = true;
		$permissoes['inserircampusuned'] = true;
		$permissoes['atendente'] = true;
		$permissoes['solicitante'] = true;
		
	} else {
		
		$sql = "SELECT 
					ur.orgid, ent.entid AS unidadeorc, tpe.orgid AS tipounidadeorc 
				FROM 
					sig.usuarioresponsabilidade ur 
				LEFT JOIN 
					entidade.entidade ent ON ent.entid = ur.entid 
				LEFT JOIN 
					entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN 
					academico.orgaouo tpe ON tpe.funid = fen.funid  
				WHERE 
					pflcod = '". $perfilid ."' 
					AND usucpf = '". $_SESSION['usucpf'] ."' 
					AND rpustatus = 'A'";
//		dbg($sql, 1);
		$orgids = (array) $db->carregar($sql);
		foreach($orgids as $tpe) {
			if($tpe['orgid']) {
				$permissoes['vertipoensino'][] = $tpe['orgid'];
			}
			if($tpe['unidadeorc']) {
				$permissoes['vertipoensino'][] = $tpe['tipounidadeorc'];
				$permissoes['verunidade'][$tpe['tipounidadeorc']][] = $tpe['unidadeorc'];
			}
		}
		
		switch($perfilid) {
			case PERFIL_ATENDENTE:
				$permissoes['atendente'] = true;
				break;
			case PERFIL_ATUALIZACAO_UNI:
			case PERFIL_ATUALIZACAO:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['inserircampusuned'] = true;
				break;
			default:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
		}
	}
	return $permissoes;
}

function validaAcessoTipoEnsino($permissoes, $orgid) {
	$permissoes = array_flip($permissoes);
	if(!isset($permissoes[$orgid])) {
		die("<script>
				alert('Você não possui autorização para acessar o TIPO DE ENSINO.');
				window.location = '?modulo=inicio&acao=C';
			 </script>");
	}
}

function filtrarcampus($dados) {
	global $db;
	$normatiza = array('campus' => 'cam.cmpid = ',
					   'orgao' => 'tpe.orgid = ',
					   'uf' => 'en.estuf = ',
					   'unidade' => 'ea.entid = ');
	
	foreach($dados as $campo => $valor) {
		$where[] = $normatiza[$campo]."'".$valor."'";
	}
	$sql ="
	 SELECT cam.cmpid FROM academico.campus cam
	 LEFT JOIN entidade.entidade e ON e.entid = cam.entid
	 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
	 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
	 LEFT JOIN entidade.entidade ea ON fea.entid = ea.entid 
	 LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = ea.entid
	 LEFT JOIN academico.orgaouo teo ON teo.funid = fen2.funid	
	 LEFT JOIN academico.orgao tpe ON teo.orgid = tpe.orgid
	 INNER JOIN entidade.endereco en ON e.entid = en.entid 	 
	 
	 WHERE ".implode(" AND ", $where);
	
	$cmpids = $db->carregar($sql);
	return $cmpids;
}


function listarCampusCadastroAjax($dados){
	global $db, $funid, $_funcoesentidade;
 	$unicod = $dados['unicod'];
 	$funid = $dados['funid'];
 	if(is_array($dados['funid'])) {
 		foreach($dados['funid'] as $f) {
		 	if($f == $_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade']) $funid_campus = $_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus'];
 			else if($f == $_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade'] || $f == 14) $funid_campus = $_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus'];
 		}
 	} else {
	 	if($dados['funid'] == $_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade']) $funid_campus = $_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus'];
 		else if($dados['funid'] == $_funcoesentidade[$_SESSION['sig_var']['orgid']]['unidade'] || $dados['funid'] == 14) $funid_campus = $_funcoesentidade[$_SESSION['sig_var']['orgid']]['campus'];
 	}
 	
	$sql = sprintf("SELECT entid AS codigo, entnome AS descricao			
					FROM entidade.entidade e 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
					WHERE fea.entid = %d					
					and fen.funid = %d 
					and e.entid NOT IN(SELECT entid FROM academico.campus)
					ORDER BY descricao",
			$unicod, $funid_campus, $unicod);
	$db->monta_combo( 'cmpid', $sql, 'S', 'Selecione', 'listarMunicipiosCadastro', '','','','','cmpid' );
	echo obrigatorio(); 
	exit;
}

function listarMunicipioCadastroAjax($dados){
	global $db; 	
 	$cmpid = $dados['cmpid']; 	
	$sql = sprintf("SELECT 
					 en.estuf AS uf,
					 m.mundescricao AS municipio
					FROM 
					 entidade.entidade e
					INNER JOIN entidade.endereco en ON( e.entid = en.entid AND en.tpeid = 1)
					INNER JOIN territorios.municipio m on m.muncod = en.muncod 
					WHERE e.entid = %d
					ORDER BY 
					 municipio", 
			$cmpid);
	$dadosr = $db->pegaLinha($sql);	
	$saida = $dadosr['uf']."|".$dadosr['municipio'];	
	echo $saida;
	exit;
}

function carregardadosmenusig() {
	// monta menu padrão contendo informações sobre as entidades, personalizado
	$menu[] = array("id" => 1, "descricao" => "Lista de unidades", "link" => "/sig/sig.php?modulo=inicio&acao=C&orgid=".$_SESSION['sig_var']['orgid']);
	if($_SESSION['sig_var']['iscampus'] == 'sim') {
		
		// Verificando o tipoensino
		switch($_SESSION['sig_var']['orgid']) {
			case TPENSPROF:
				$menu[] = array("id" => 2, "descricao" => "Tabela da uned", "link" => "/sig/sig.php?modulo=principal/editarcampus&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&cmpid=".$_SESSION['sig_var']['cmpid']);
				break;
			case TPENSSUP:
				$menu[] = array("id" => 2, "descricao" => "Tabela do campus", "link" => "/sig/sig.php?modulo=principal/editarcampus&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&cmpid=".$_SESSION['sig_var']['cmpid']);
				break;
		}
		
	} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
		$menu[] = array("id" => 2, "descricao" => "Tabela de entidade", "link" => "/sig/sig.php?modulo=principal/editarentidade&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&entid=".$_SESSION['sig_var']['entid']);
	}
	
	$menu[] = array("id" => 3, "descricao" => "Dados da entidade", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=ent");
	$menu[] = array("id" => 4, "descricao" => "Dados do dirigente", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=dir");
	$menu[] = array("id" => 5, "descricao" => "Interlocutor Institucional", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=int");
	$menu[] = array("id" => 6, "descricao" => "Dados específicos", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=esp");
	
	if($_SESSION['sig_var']['iscampus'] == 'nao') {
		// Verificando o tipoensino
		switch($_SESSION['sig_var']['orgid']) {
			case TPENSPROF:
				$menu[] = array("id" => 7, "descricao" => "Lista de Uned", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=cam");
				break;
			case TPENSSUP:
				$menu[] = array("id" => 7, "descricao" => "Lista de campus", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=cam");
				break;
		}
	}
	$menu[] = array("id" => 8, "descricao" => "Obras", "link" => "/sig/sig.php?modulo=principal/inserir_entidade&acao=A&page=obr");
	$menu[] = array("id" => 9, "descricao" => "Nota técnica", "link" => "/sig/sig.php?modulo=principal/notatecnica&acao=A");
	
	return $menu;
}
function salvarRegistroEntidade($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->salvar();
	
	echo '<script type="text/javascript">
			alert("Dados gravados com sucesso");
			window.location = \'?modulo=principal/inserir_entidade&acao=A&page=ent\';
		  </script>';
	exit;
	
}
function salvarRegistroDirigente($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->adicionarFuncoesEntidade($dados['funcoes']);
	$entidade->salvar();
    
    echo '<script type="text/javascript">
    		alert("Dados gravados com sucesso");
	        window.location = \'?modulo=principal/inserir_entidade&acao=A&page=dir\';
	      </script>';
    exit;
}
function salvarRegistroInterlocutor($dados) {
	global $db;
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->adicionarFuncoesEntidade($dados['funcoes']);
	$entidade->salvar();
    
    echo '<script type="text/javascript">
    		alert(\'Dados gravados com sucesso\');
	        window.location = \'?modulo=principal/inserir_entidade&acao=A&page=int\';
	      </script>';
    exit;
}
function salvarRegistroDetalhes($dados) {
	global $db;
	if($_SESSION['sig_var']['iscampus'] == 'sim') {
		/* Ajustando datas (mm/yyyy)
	 	 * Solicitado pelo Hugo (devido ordenação)
	 	 * formato de saída : yyyymm 
	 	 */
		$dados['cmpdataimplantacao'] = substr($dados['cmpdataimplantacao'],3,4).substr($dados['cmpdataimplantacao'],0,2);
		
		// Atualizando "campus"
		$sql = "UPDATE academico.campus SET cmpobs = '". substr($dados['cmpobs'],0,1000) ."',
			    cmpdataatualizacao = NOW(),
				usucpf = '".$_SESSION['usucpf']."',
				cmpdataimplantacao='".$dados['cmpdataimplantacao']."', 
       			cmpdatainauguracao=".(($dados['cmpdatainauguracao'])?"'".formata_data_sql($dados['cmpdatainauguracao'])."'":"NULL").", 
       			cmpexistencia='".$dados['cmpexistencia']."', 
       			cmpsituacao='".$dados['cmpsituacao']."', 
       			cmpinstalacao='".$dados['cmpinstalacao']."', 
       			cmpsituacaoobra='".$dados['cmpsituacaoobra']."'
				WHERE cmpid = '". $dados['cmpid'] ."'";
		$db->executar($sql);
	}

	$is_ent = $db->pegaUm("SELECT entid FROM academico.entidadedetalhe WHERE entid='".$_SESSION['sig_var']['entid']."'");
	if($is_ent) {
		$db->executar("UPDATE academico.entidadedetalhe
					   SET edtdsc='". $dados['edtdsc'] ."'
 					   WHERE entid='".$_SESSION['sig_var']['entid']."'");
	} else {
		$db->executar("INSERT INTO academico.entidadedetalhe(
					   entid, edtdsc)
    				   VALUES ('".$_SESSION['sig_var']['entid']."', '". $dados['edtdsc'] ."');");
	}
	$db->commit();
    echo '<script type="text/javascript">
		        window.location = \'?modulo=principal/inserir_entidade&acao=A&page=esp\';
	      </script>';
    exit;
}

function monta_cabecalho_sig($entid) {
	global $db;
	
	$titulo_modulo = "SIG";
	monta_titulo( $titulo_modulo,'Sistema de Informações Gerenciais');
	if($_SESSION['sig_var']['iscampus'] == 'sim') {
		$sql = "SELECT ent.entnome as campus, ende.estuf, mundescricao, orgdesc, uo.entnome AS unidadeorc, uo.entid as unidadeorcid, tpe.orgid FROM entidade.entidade ent 
				inner JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				inner JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
				inner JOIN entidade.entidade uo ON uo.entid = fea.entid 
				inner JOIN entidade.funcaoentidade fen2 ON fen2.entid = uo.entid
				inner JOIN academico.orgaouo teu ON teu.funid = fen2.funid  
				inner JOIN academico.orgao tpe ON tpe.orgid = teu.orgid 
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
				WHERE ent.entid = '". $entid ."' ORDER BY ent.entnome";
	} else {
		$sql = "SELECT ent.entid as unidadeorcid, ent.entnome as unidadeorc, ende.estuf, mundescricao, orgdesc, tpe.orgid FROM entidade.entidade ent 
				inner JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
				inner JOIN academico.orgaouo teu ON teu.funid = fen.funid  
				inner JOIN academico.orgao tpe ON tpe.orgid = teu.orgid 
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
				WHERE ent.entid = '". $entid ."' ORDER BY ent.entnome";
	}

	$dadosentidade = $db->pegaLinha($sql);
	if($dadosentidade && $dadosentidade['orgdesc']) {
		echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
		echo "<tr>";
		echo "<td class='SubTituloDireita'>Tipo Ensino :</td><td>".$dadosentidade['orgdesc']."</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td class='SubTituloDireita'>Unidade Orçamentária :</td><td><a style=\"cursor:pointer;\" onclick=\"window.location='?modulo=principal/editarentidade&acao=A&iscampus=nao&orgid=".$dadosentidade['orgid']."&entid=".$dadosentidade['unidadeorcid']."';\"><img src=\"../imagens/consultar.gif\" border=\"0\"> ".$dadosentidade['unidadeorc']."</a></td>";
		echo "</tr>";
		if($_SESSION['sig_var']['iscampus'] == 'sim') {
			echo "<tr>";
			echo "<td class='SubTituloDireita'>Campus / Uned :</td><td>".$dadosentidade['campus']."</td>";
			echo "</tr>";
		}
		echo "<tr>";
		echo "<td class='SubTituloDireita'>UF / Munícipio :</td><td>".$dadosentidade['estuf']." / ".$dadosentidade['mundescricao']."</td>";
		echo "</tr>";
		echo "</table>";
	} else {
		die("<script>
				alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
				window.location='?modulo=inicio&acao=C';
			 </script>");
	}
}

function salvarProcessoSeletivo($dados) {
	global $db;
	/*
	 * prsnrvagas - apesar do javascript esta com a mascara, estava chegando número com "."(ponto), isto ocasionava erros. Estou filtrando via PHP, para confirmar a inserção
	 */
	if($dados['prsid']) {
		$sql = "UPDATE academico.processoseletivo
				SET prsinscricaofim='".formata_data_sql($dados['prsinscricaofim'])."', 
   				prsinscricaoini='".formata_data_sql($dados['prsinscricaoini'])."', 
   				prsprovaini='".formata_data_sql($dados['prsprovaini'])."', 
       			prsprovafim='".formata_data_sql($dados['prsprovafim'])."', 
       			prsinicioaula='".formata_data_sql($dados['prsinicioaula'])."',
       			prsnrvagas=".(($dados['prsnrvagas'])?"'".str_replace(array(".",","),array("",""),$dados['prsnrvagas'])."'":"NULL")." 
 				WHERE prsid='".$dados['prsid']."'";
		$db->executar($sql);
	} elseif($dados['rmprsid']) {
		$db->executar("DELETE FROM academico.processoseletivo WHERE prsid='".$dados['rmprsid']."'");
	} else {
		$cmpid = $db->pegaUm("SELECT cmpid FROM academico.campus WHERE entid = '".$_SESSION['sig_var']['entid']."'");
		$sql="INSERT INTO academico.processoseletivo(
       	    	cmpid, prsinscricaofim, prsinscricaoini, prsprovaini, 
           		prsprovafim, prsinicioaula, prsnrvagas)
	   		  VALUES ('".$cmpid."', 
    				  '".formata_data_sql($dados['prsinscricaofim'])."', 
    				  '".formata_data_sql($dados['prsinscricaoini'])."', 
    				  '".formata_data_sql($dados['prsprovaini'])."', 
            		  '".formata_data_sql($dados['prsprovafim'])."', 
            		  '".formata_data_sql($dados['prsinicioaula'])."',
            		  ".(($dados['prsnrvagas'])?"'".str_replace(array(".",","),array("",""),$dados['prsnrvagas'])."'":"NULL").");";
		$db->executar($sql);
	}
	$db->commit();
    echo '<script type="text/javascript">
	        window.location = \'?modulo=principal/inserir_entidade&acao=A&page=esp\';
	      </script>';
    exit;
}

function atualizardadoscampus($dados) {
	global $db;
	
	$sql_excluir = "DELETE FROM academico.campusitem WHERE cmpid='". $dados['cmpid']."'";
	$db->executar($sql_excluir);
	$sqls = processaInsercaoCampusItem($_POST);
	if($sqls) {
		foreach($sqls as $sql) {
			$db->executar($sql);
		}
	}
	// Inserindo os dados do curso
	if($dados['cursos']) {
		foreach($dados['cursos'] as $curid => $val) {
			foreach($val as $ano => $cpcqtd) {
				$sql_update = "UPDATE academico.campuscurso SET cpcqtd=".(($cpcqtd !== "")?"'".$cpcqtd."'":"NULL")." WHERE cpcano='".$ano."' AND curid='".$curid."' AND cmpid='".$dados['cmpid']."'";
				$db->executar($sql_update);
			}
		}
	}
		
	$db->commit();
	echo "<script>
			alert('Os dados do campus foram atualizados com sucesso.');
			window.location = '?modulo=principal/editarcampus&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&cmpid=".$dados['cmpid']."';
		  </script>";
	exit;
	
}

function carregarItensEntidadeVisualizar($dados) {
	global $db, $anosanalisados, $tituloitens;
	if($dados['porcampus']=='sim') {
		$listacampus = $db->carregar("SELECT ent.entid, ent.entnome, cmp.cmpid FROM entidade.entidade ent
									  LEFT JOIN academico.campus cmp ON cmp.entid = ent.entid 
									  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
									  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
									  WHERE fea.entid='".$_SESSION['sig_var']['entid']."' AND cmp.cmpid IS NOT NULL");
		if($listacampus[0]) {
			foreach($listacampus as $campus) {
			?>
			<table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr><td class="SubTituloCentro"><? echo $campus['entnome']; ?></td></tr>
			<tr><td class="SubTituloEsquerda"><? echo (($tituloitens[$_SESSION['sig_var']['orgid']])?$tituloitens[$_SESSION['sig_var']['orgid']]:$tituloitens['default']); ?></td></tr>
			<tr><td><?
			// 		Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
			if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
				$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
			} else {
				$anos = $anosanalisados['default'];
			}
			unset($cabecalho,$paramselects);
			$cabecalho[] = "Itens";
			foreach($anos as $ano) {
				$paramselects[] = "'<input class=\"normal\" id=\"' || itm.itmid || '".$ano."\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || '][". $ano ."]\" '|| 
									CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' 
									ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' 
									END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"14\" type=\"hidden\" value=\"' || 
									CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $campus['cmpid'] ."') is null 
									THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $campus['cmpid'] ."') END || '\">' AS ano_".$ano;
				$cabecalho[] = $ano;		
			}
			$paramselects = implode(",",$paramselects);
			// 	criando o SELECT
			$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
					". $paramselects ."
					FROM academico.item itm 
					LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
					WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = false
					ORDER BY tei.teiordem";
			$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
			?></td></tr>
			<tr><td class="SubTituloEsquerda">Situação Atual</td></tr>
			<tr><td><?
			unset($cabecalho);
			$cabecalho = array("Itens", "Atual");
			$paramselct = "'<input  class=\"normal\" id=\"' || itm.itmid || ' \" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || ']\" '|| 
					CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' 
						ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' 
						END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" type=\"hidden\" value=\"' || 
					CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $campus['cmpid'] ."') is null 
						THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $campus['cmpid'] ."') END || '\">' AS ano";
	
			$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>',
					". $paramselct ."
					FROM academico.item itm 
					LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
					WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = true
					ORDER BY tei.teiordem";
			$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
			?></td></tr>
			</table>
			<?
			}
		} else {
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr><td class=\"SubTituloCentro\">Não exitem campus associados.</td></tr>";
			echo "</table>";
		}
	} else {
	?>
	<table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr><td class="SubTituloEsquerda"><? echo (($tituloitens[$_SESSION['sig_var']['orgid']])?$tituloitens[$_SESSION['sig_var']['orgid']]:$tituloitens['default']); ?></td></tr>
	<tr><td><?
	// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
	if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
		$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	unset($cabecalho);
	$cabecalho[] = "Itens";
	foreach($anos as $ano) {
		$paramselects[] = "'<input name=\"gravacaocampo_'|| itm.itmid ||'_".$ano."\" type=\"hidden\" value=\"'|| CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi 
																			 LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
															  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
															  			     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
															  			     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
															  			     WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '".$ano."' AND fea.entid = '".$_SESSION['sig_var']['entid']."') as varchar) is null 
															  THEN '' 
															  ELSE 	   cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
															  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
															  			     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
															  			     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
															  			     WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '".$ano."' AND fea.entid = '".$_SESSION['sig_var']['entid']."') as varchar)
															  END || '\" 
							'|| CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"'	END ||'  
							maxlength=\"' || tpi.tpitamanhomax || '\" 
							size=\"14\" 
							class=\"normal\" readonly> ' AS ano_".$ano;
		$cabecalho[] = $ano;		
	}
	$paramselects = implode(",",$paramselects);
	// criando o SELECT
	$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
			". $paramselects ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = false
			ORDER BY tei.teiordem";
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?></td></tr>
	<tr><td class="SubTituloEsquerda">Situação Atual</td></tr>
	<tr><td><?
	unset($cabecalho);
	$cabecalho = array("Itens", "Atual");
	$paramselct = "'<input name=\"gravacaocampo_'|| itm.itmid ||'\" type=\"hidden\" value=\"'|| CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
															  	     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
															  	     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
															  	     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
															  		 WHERE itm.itmid = cpi.itmid AND fea.entid = '".$_SESSION['sig_var']['entid']."') as varchar) is null 
													  THEN '' 
													  ELSE 	   cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
															  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
															  	     		 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
															  	             LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
															  			     WHERE itm.itmid = cpi.itmid AND fea.entid = '".$_SESSION['sig_var']['entid']."') as varchar)
													  END || '\" 
				   '|| CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"'	END ||'  
				   maxlength=\"' || tpi.tpitamanhomax || '\" 
				   size=\"14\" 
				   class=\"normal\" readonly> ' AS ano";
	
	$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>',
			". $paramselct ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = true
			ORDER BY tei.teiordem";
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?></td></tr>
	</table>
	<?
	}
	exit;
}

function mascara($mask, $valor) {
	$valor = str_replace(".","",$valor);
	$k = strlen($valor);
	for($i=strlen($mask);$i>0;$i--) {
		if($mask[$i] == "#") {
			while(is_numeric($valor[($k-1)])){
				$vlrmask[] =  $valor[($k-1)];
				$k--;
				break;
			}
		} else {
			$vlrmask[] = $mask[$i];
		}
		if($k == 0)break;
	}
	$vlrmask = array_reverse($vlrmask);
	return implode("", $vlrmask);
}

function salvarObrasInauguradas($dados) {
	global $db;
	$dadosobrinau = explode(";",$dados['param']);
	$db->executar("DELETE FROM academico.obrainauguradacampus WHERE cmpid='".$dadosobrinau[0]."' AND obrid='".$dadosobrinau[1]."'");
	$db->executar("INSERT INTO academico.obrainauguradacampus(cmpid, obrid) VALUES ('".$dadosobrinau[0]."', '".$dadosobrinau[1]."');");
	$db->commit();
	exit;		
}
function removerObrasInauguradas($dados) {
	global $db;
	$dadosobrinau = explode(";",$dados['param']);
	$db->executar("DELETE FROM academico.obrainauguradacampus WHERE cmpid='".$dadosobrinau[0]."' AND obrid='".$dadosobrinau[1]."'");
	$db->commit();
	exit;		
}

function definirtipolocalidade($orgid) {
	switch($_SESSION['sig_var']['orgid']) {
		case TPENSSUP:
			$tipolocalidade['nome']  = "Campus";
			$tipolocalidade['artigo+nome'] = "o Campus";
			break;
		case TPENSPROF:
			$tipolocalidade['nome']  = "Uned";
			$tipolocalidade['artigo+nome'] = "a Uned";
			break;
	}
	return $tipolocalidade;
}

function buscarCnpj($dados) {
	global $db;
	ob_end_clean();
    $entidade = Entidade::carregarEntidadePorCnpjCpf(str_replace(array(".","/"),array("",""),$dados['entnumcpfcnpj']), $db->testa_superuser());
    if ($entidade->getPrimaryKey() !== null) {
    	die($entidade->getPrimaryKey());
    } else {
        die('0');
    }
}

function salvarNovoCampusUned($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->adicionarFuncoesEntidade($dados['funcoes']);
	$entidade->salvar();
	/*
	 * Inserindo o campus no sig
	 */
    $sql = "INSERT INTO academico.campus(entid, cmpdataatualizacao, usucpf)
    		VALUES ('".$entidade->getEntid()."', NOW(), '".$_SESSION['usucpf']."');";
    $db->executar($sql);
    $db->commit();
    echo '<script type="text/javascript">
	        window.location = \'?modulo=principal/inserir_entidade&acao=A&page=cam\';
	      </script>';
    exit;

}
?>