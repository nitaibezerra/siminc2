<?
function sqlEquipeCoordenadorAdjunto($dados) {
	global $db;
	
	$sql = "
			(
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo, 
					(FLOOR((t.tpeatuacaofim - t.tpeatuacaoinicio)/30)+1) as nmeses, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=p.pflcod) as perfil
			FROM sisindigena2.identificacaousuario i
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."',
							  '".PFL_SUPERVISORIES."',
							  '".PFL_COORDENADORLOCAL."',
							  '".PFL_PROFESSORALFABETIZADOR."',
							  '".PFL_ORIENTADORESTUDO."',
							  '".PFL_CONTEUDISTA."',
							  '".PFL_PESQUISADOR."') AND i.iusstatus='A' AND i.picid='".$dados['picid']."' ORDER BY p.pflcod, i.iusnome
			
			)
			
			";
	
	return $sql;
}

function carregarCoordenadorAdjuntoIES($dados) {
	global $db;
	
	if($dados['iusd']) $cl = "INNER JOIN sisindigena2.identificacaousuario i ON i.picid = n.picid AND i.iusd = '".$dados['iusd']."'";
	if($dados['picid']) $cl = "WHERE n.picid='".$dados['picid']."'";
	
	$arr = $db->pegaLinha("SELECT n.docid, n.picsede, n.picid, u.uncid, su.uniuf, su.unisigla||' - '||su.uninome||' >> '||su2.unisigla||' - '||su2.uninome as descricao 
						   FROM sisindigena2.nucleouniversidade n  
						   INNER JOIN sisindigena2.universidadecadastro u ON u.uncid = n.uncid  
					 	   INNER JOIN sisindigena2.universidade su 		 ON su.uniid = u.uniid 
					 	   INNER JOIN sisindigena2.universidade su2       ON su2.uniid = n.uniid
						   {$cl}");
	
	$docid = $arr['docid'];
	
	if(!$docid) {
		$docid = wf_cadastrarDocumento(TPD_COORDENADORIES,"SIS Indigena Coordenador Adjunto IES ".$arr['picid']);
		$db->executar("UPDATE sisindigena2.nucleouniversidade SET docid='".$docid."' WHERE picid='".$arr['picid']."'");
		$db->commit();
	}
	
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sisindigena2.identificacaousuario i 
							   INNER JOIN sisindigena2.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.picid='".$arr['picid']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'");
	
	$_SESSION['sisindigena2']['coordenadoradjuntoies'] = array("docid" => $docid,"picsede" => $arr['picsede'],"picid" => $arr['picid'],"descricao" => $arr['descricao']."( ".$infprof['iusnome']." )", "uncid" => $arr['uncid'], "reiid" => $arr['reiid'], "estuf" => $arr['uniuf'], "iusd" => $infprof['iusd'], "iuscpf" => $infprof['iuscpf']);	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function gravarProjetoPedagogico($dados) {
	global $db;
	
	$sql = "DELETE FROM sisindigena2.listaabrangenciaacaonucleo WHERE picid='".$dados['picid']."' AND laaid IS NOT NULL";
	$db->executar($sql);
	if($dados['laaid']) {
		foreach($dados['laaid'] as $laaid) {
			
			$sql = "INSERT INTO sisindigena2.listaabrangenciaacaonucleo(
		            picid, laaid)
		    		VALUES ('".$dados['picid']."', '".$laaid."');";
			
			$db->executar($sql);
			
		}
	
	}
	
	if($_FILES['arquivo']['error']=='0') {
		$campos	= array("picid"	 => $dados['picid']);	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("listaabrangenciaacaonucleo", $campos ,"sisindigena2");
				
		$arquivoSalvo = $file->setUpload($dados['mapadsc']);
		
	}
	
	
	$db->commit();
	
	
	
	$sql = "DELETE FROM sisindigena2.eixospedagogicosnucleo WHERE picid='".$dados['picid']."'";
	$db->executar($sql);
	
	if($dados['expid']) {
		foreach($dados['expid'] as $expid) {
			
			$sql = "INSERT INTO sisindigena2.eixospedagogicosnucleo(
		            expid, picid, eprobs)
		    		VALUES ('".$expid."', '".$dados['picid']."', '".$dados['eprobs_'.$expid]."');";
			
			$db->executar($sql);
			
		}
	}
	
	$sql = "UPDATE sisindigena2.nucleouniversidade SET picsituacaosociolinguistica=".(($dados['picsituacaosociolinguistica'])?"'".$dados['picsituacaosociolinguistica']."'":"NULL").", picmetodologiaaplicada=".(($dados['picmetodologiaaplicada'])?"'".$dados['picmetodologiaaplicada']."'":"NULL").", picmetodologiaavaliacao=".(($dados['picmetodologiaavaliacao'])?"'".$dados['picmetodologiaavaliacao']."'":"NULL").", picmetodologiaacompanhamento=".(($dados['picmetodologiaacompanhamento'])?"'".$dados['picmetodologiaacompanhamento']."'":"NULL")." WHERE picid='".$dados['picid']."'";
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert"=>"Operação realizada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
}

function inserirAldeia($dados) {
	global $db;
	$sql = "INSERT INTO sisindigena2.listaabrangenciaacaonucleo(
            picid, lanaldeiadsc)
    		VALUES ('".$dados['picid']."', '".$dados['lanaldeiadsc']."');";
	
	$db->executar($sql);
	$db->commit();
}

function removerAbrangenciaAcao($dados) {
	global $db;
	
	$sql = "DELETE FROM sisindigena2.listaabrangenciaacaonucleo WHERE lanid='".$dados['lanid']."'";
	$db->executar($sql);
	$db->commit();
}


function quadroAbrangenciaAcao($dados) {
	global $db;
	
	switch($dados['grid']) {
		case 'escola_atendida':
			if($dados['visrelatorio']) {
				
				$sql = "SELECT '<img src=../imagens/seta_filho.gif align=absmiddle> '||lanescoladsc FROM sisindigena2.listaabrangenciaacaonucleo WHERE lanescoladsc IS NOT NULL AND picid='".$dados['picid']."' ORDER BY lanid DESC";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
				
			} else {
				echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
				echo '<tr><td colspan="3"><input type=radio name=busca value=1 onclick=exibeBusca(this);> Buscar pelo EDUCACENSO <input type=radio name=busca value=2 onclick=exibeBusca(this);> Buscar escola que não existe no EDUCACENSO</td></tr>';
				echo '<tr id="tr_educa" style="display:none;"><td class="SubTituloDireita">UF:</td><td>';
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFProjetoPedagogico', '', '', '', 'S', 'estuf');
				echo '</td><td id="td_municipio"></td><td id="td_escola"></td></tr>';
				echo '<tr id="tr_neduca" style="display:none;"><td class="SubTituloDireita">Digite:</td><td colspan="2"><input type="text" style="text-align:;" name="lanescoladsc" size="61" maxlength="60" value="" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="lanescoladsc" title="Nome" class="obrigatorio normal"> <input type="button" name="inserir" value="Inserir" onclick="inserirEscola(jQuery(\'#lanescoladsc\').val());"></td></tr>';
				echo '</table>';
				
				echo '<br>';
				echo "<div  style=\"height:70px;overflow:auto;\">";
				$sql = "SELECT '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirRegistro('||lanid||',\'escola_atendida\');\">' as r, lanescoladsc FROM sisindigena2.listaabrangenciaacaonucleo WHERE lanescoladsc IS NOT NULL AND picid='".$dados['picid']."' ORDER BY lanid DESC";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
				echo "</div>";
			}
			
			break;
		
		case 'aldeia':
			if($dados['visrelatorio']) {
				
				$sql = "SELECT '<img src=../imagens/seta_filho.gif align=absmiddle> '||lanaldeiadsc FROM sisindigena2.listaabrangenciaacaonucleo WHERE lanaldeiadsc IS NOT NULL AND picid='".$dados['picid']."' ORDER BY lanid DESC";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
				
			} else {
				echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
				echo '<tr><td class="SubTituloDireita">Digite:</td><td><input type="text" style="text-align:;" name="lanaldeiadsc" size="61" maxlength="60" value="" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="lanaldeiadsc" title="Nome" class="obrigatorio normal"> <input type="button" name="inserir" value="Inserir" onclick="inserirAldeia();"></td></tr>';
				echo '</table>';
				echo '<br>';
				echo "<div  style=\"height:70px;overflow:auto;\">";
				$sql = "SELECT '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirRegistro('||lanid||',\'aldeia\');\">' as r, lanaldeiadsc FROM sisindigena2.listaabrangenciaacaonucleo WHERE lanaldeiadsc IS NOT NULL AND picid='".$dados['picid']."' ORDER BY lanid DESC";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
				echo "</div>";
			}
			
			break;
		case 'mapa':
			if($dados['visrelatorio']) {
				
				$sql = "SELECT 
						'<img src=../imagens/seta_filho.gif align=absmiddle> <img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"window.location=window.location+\'&requisicao=downloadArquivoAbrangenciaAcao&arqid='||a.arqid||'\'\"> '||a.arqnome||'.'||a.arqextensao as arquivo, a.arqdescricao as descricao 
						FROM sisindigena2.listaabrangenciaacaonucleo l 
						INNER JOIN public.arquivo a ON a.arqid = l.arqid 
						WHERE picid='".$dados['picid']."'";
				
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
				
			} else {
				
				echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
				echo '<tr><td class="SubTituloDireita">Arquivo:</td><td><input type="file" name="arquivo"> <input type="button" name="inserir" value="Inserir" onclick="salvarProjetoPedagogico(\'sisindigena2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=projetopedagogico\');"></td></tr>';
				echo '<tr><td class="SubTituloDireita">Descrição:</td><td><textarea id="mapadsc" name="mapadsc" cols="60" rows="2" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="width:45 ex;"></textarea></td></tr>';
				echo '</table>';
				echo '<br>';
				echo "<div  style=\"height:70px;overflow:auto;\">";
				$sql = "SELECT 
						'<img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"window.location=window.location+\'&requisicao=downloadArquivoAbrangenciaAcao&arqid='||a.arqid||'\'\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirRegistro('||lanid||',\'mapa\');\">' as r, a.arqnome||'.'||a.arqextensao as arquivo, a.arqdescricao as descricao 
						FROM sisindigena2.listaabrangenciaacaonucleo l 
						INNER JOIN public.arquivo a ON a.arqid = l.arqid 
						WHERE picid='".$dados['picid']."'";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
				echo "</div>";
				
			}
			break;
		default:
			if($dados['visrelatorio']) {
				
				$sql = "SELECT '<img src=../imagens/seta_filho.gif align=absmiddle> '||li.laadsc 
						FROM sisindigena2.listaabrangenciaacaonucleo l 
						INNER JOIN sisindigena2.listaabrangenciaacao li ON li.laaid = l.laaid 
						WHERE picid='".$dados['picid']."' AND li.laatipo='".$dados['laatipo']."'";
				
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
			} else {
				echo "<div  style=\"height:100px;overflow:auto;\">";
				$sql = "SELECT '<input type=\"checkbox\" name=\"laaid[]\" value=\"'||laaid||'\" '||COALESCE((SELECT CASE WHEN lanid IS NULL THEN '' ELSE 'checked' END FROM sisindigena2.listaabrangenciaacaonucleo WHERE laaid=l.laaid AND picid='".$dados['picid']."'),'')||'>' as chk, laadsc FROM sisindigena2.listaabrangenciaacao l WHERE laatipo='".$dados['laatipo']."' ORDER BY laadsc";
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
				echo "</div>";
			}
	}
	
}

function carregarEscolasPorMunicipio($dados) {
	global $db;
	
	$sql = "SELECT pk_cod_entidade||' - '||no_entidade as codigo, pk_cod_entidade||' - '||no_entidade as descricao FROM educacenso_2013.tab_entidade WHERE fk_cod_municipio='".$dados['muncod']."'";
	$db->monta_combo('escola', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'escola');
	
	echo ' <input type="button" name="inserir" value="Inserir" onclick="inserirEscola(jQuery(\'#escola option:selected\').text());">';
	
}


function downloadArquivoAbrangenciaAcao($dados) {
	ob_clean();
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "listaabrangenciaacaonucleo", NULL, "sisindigena" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function inserirEscola($dados) {
	global $db;
	
	$sql = "INSERT INTO sisindigena2.listaabrangenciaacaonucleo(
            picid, lanescoladsc)
    		VALUES ('".$dados['picid']."', '".$dados['lanescoladsc']."');";
	
	$db->executar($sql);
	$db->commit();
	
}


function carregarRedeTerritorio($dados) {
	global $db;
	
	$sql = "SELECT retid as codigo, 
				   CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao||' ( Municipal )' 
				   		WHEN e.estuf IS NOT NULL THEN e.estuf||' / '||e.estdescricao||' ( Estadual )'
				   END as descricao 
			FROM sisindigena2.redeterritorios i 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT JOIN territorios.estado e ON e.estuf = i.estuf 
			ORDER BY descricao";
	
	$db->monta_combo('retid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'retid','', '');
	
	
}

function carregarListaCustos($dados) {
	global $db;

	$sql = "SELECT ".(($dados['consulta'])?"''":"'<center><img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirCustos(\''||o.orcid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirCustos(\''||o.orcid||'\');\"></center>'")." as acao, uu.unisigla||' - '||uu.uninome as universidade, g.gdedesc, 'Verba', o.orcvlrunitario, replace(o.orcdescricao,'\n','<br>')
			FROM sisindigena2.orcamento o
			INNER JOIN sisindigena2.grupodespesa g ON g.gdeid = o.gdeid 
			INNER JOIN sisindigena2.nucleouniversidade un ON un.picid = o.picid 
			INNER JOIN sisindigena2.universidade uu ON uu.uniid = un.uniid 
			WHERE o.orcstatus='A' ".(($dados['picid'])?" AND o.picid='".$dados['picid']."'":"")." ".(($dados['uncid'])?" AND un.uncid='".$dados['uncid']."'":"")."
			ORDER BY g.gdedesc";

	$cabecalho = array("&nbsp;","IES","Grupo de Despesa","Unidade de Medida","Valor total (R$)","Detalhamento");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');

}


function carregarNaturezaDespesasCustos($dados) {
	global $db;
	$sql = "SELECT n.ndecodigo, n.ndedesc, SUM(o.orcvlrtotal) as total
			FROM sisindigena2.orcamento o
			INNER JOIN sisindigena2.itemdespesa i ON i.ideid = o.ideid
			INNER JOIN sisindigena2.grupodespesa g ON g.gdeid = i.gdeid
			INNER JOIN sisindigena2.naturezadespesa n ON n.ndeid = g.ndeid
			WHERE o.picid='".$dados['picid']."' AND o.orcstatus='A'
			GROUP BY n.ndecodigo, n.ndedesc";

	$cabecalho = array("Código","Descrição","Valor(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarOrcamento($dados) {
	global $db;
	$sql = "SELECT * FROM sisindigena2.orcamento o
			INNER JOIN sisindigena2.grupodespesa g ON o.gdeid = g.gdeid
			WHERE orcid='".$dados['orcid']."'";

	$orcamento = $db->pegaLinha($sql);

	return $orcamento;

}

function atualizarCusto($dados) {
	global $db;
	$sql = "UPDATE sisindigena2.orcamento SET gdeid='".$dados['gdeid']."', orcvlrunitario='".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', orcdescricao='".$dados['orcdescricao']."'
			WHERE orcid='".$dados['orcid']."'";

	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Custo inserido com sucesso","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);

}

function excluirCustos($dados) {
	global $db;
	$sql = "DELETE FROM sisindigena2.orcamento WHERE orcid='".$dados['orcid']."'";
	$db->executar($sql);
	$db->commit();


}

function inserirCusto($dados) {
	global $db;
	
	$sql = "INSERT INTO sisindigena2.orcamento(
            picid, gdeid, orcvlrunitario,
            orcstatus, orcdescricao)
    		VALUES ('".$dados['picid']."', '".$dados['gdeid']."', '".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', 'A', '".$dados['orcdescricao']."');";

	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Item inserido com sucesso.","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);

}



?>