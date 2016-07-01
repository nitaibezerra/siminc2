<?php
if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	exit;
}

if($_REQUEST['requisicao']){
	$arrDados = $_REQUEST['requisicao']();
	if($arrDados){
		extract($arrDados);
	}
}

function exibirObrasEmpreendimento()
{
	global $db;
	extract($_POST);
	if($_POST['obrtextobusca']){	
		$filtro = "( removeacento(upper(oi.obrdesc)) ilike removeacento(upper('%{$_POST["obrtextobusca"]}%')) OR ";
		$filtro .= " upper(ee.entnome) ilike upper('%{$_POST["obrtextobusca"]}%') OR ";
		$filtro .= " upper(mpi.plicod) ilike upper('%{$_POST["obrtextobusca"]}%') OR ";
		$filtro .= " oi.obrid::text ilike ('%".(int)$_POST["obrtextobusca"]."%') )";
		$arrWhere[] = $filtro;	
	}
	if($_POST['mundsc']){	
		$filtro = "removeacento(upper(tm.mundescricao)) ilike removeacento(upper('%{$_POST["mundsc"]}%'))";
		$arrWhere[] = $filtro;
		
	}
	if($_POST['estuf']){	
		$filtro = "tm.estuf = '{$_POST["estuf"]}'";
		$arrWhere[] = $filtro;
		
	}
	$sql = "SELECT DISTINCT
					CASE WHEN arqfoto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,					
					CASE WHEN restricao IS NOT NULL THEN
						case when (SELECT COUNT(rstoid) FROM obras.restricaoobra WHERE obrid = oi.obrid AND (rstsituacao OR rstsituacao IS NULL) AND (rstdtsuperacao IS NULL) AND rststatus = 'A' ) = 0 then
							'<img src=\"/imagens/obras/atencao_verde.png\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'restricao\');\" style=\"cursor:pointer; width:15px;\" title=\"Restrições Superadas\">'
						else
							'<img src=\"/imagens/obras/atencao.png\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'restricao\');\" style=\"cursor:pointer; width:15px;\" title=\"Restrições Pendende\">'
						end
					ELSE
						''
					END as restricoes,				
					oi.obrid as id,
					oi.preid as idpreobra,
					oi.numconvenio,
					--dcoano,
					'<a onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'cadastro\');\">' || upper(oi.obrdesc) || '</a>'  
					AS nome,
					ee.entnome as entdescricao,
					tm.mundescricao || ' / ' || ed.estuf as municipiouf,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					stodesc as situacao,
					-- pog ordena data
					CASE 
						WHEN ov.supdtinclusao IS NOT NULL THEN 
							'<div style=\"display:none\">'||ov.supdtinclusao||'</div>'
						WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE 
						WHEN ov.supdtinclusao IS NOT NULL AND oi.stoid IN (3,6,7) THEN
							CASE WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
									CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
									ELSE
										'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
									END
								 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
								 	CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
									ELSE
										'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
									END
								 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
								 	CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
									ELSE
										'COLOR=\"#000000\" TITLE=\"Esta obra está desatualizada\">'
									END
								 ELSE 
								 	'COLOR=\"#000000\"'
							END	|| 
							to_char(ov.supdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - ov.supdtinclusao)||' dia(s) )'
						WHEN ov.supdtinclusao IS NOT NULL AND oi.stoid NOT IN (3,4,5,6,7,99) THEN
							CASE WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
									CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
									ELSE
										'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
									END
								 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
								 	CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
									ELSE
										'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
									END
								 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
								 	CASE WHEN oi.obrpercexec >= 100.00 THEN
										'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
									ELSE
										'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">'
									END
								 ELSE 
								 	'COLOR=\"#000000\"'
							END	|| 
							to_char(ov.supdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - ov.supdtinclusao)||' dia(s) )'
						WHEN oi.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
											ELSE
												'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
											END
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
										 	CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
											ELSE
												'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
											END
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
										 	CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">' 
											ELSE
												'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
						 					CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
											ELSE
												'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
											END
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
										 	CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
											ELSE
												'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
											END
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
										 	CASE WHEN oi.obrpercexec >= 100.00 THEN
												'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">' 
											ELSE
												'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN oi.stoid IN (3) THEN
						  	'COLOR=\"#000000\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					(select to_char( max(supvdt),'DD/MM/YYYY') from obras.supervisao where obrid = oi.obrid and supstatus = 'A') as dtvistoria,
					--fim pog
					
					CASE WHEN COALESCE(ov.supervisao,0)=0 then 'Sem Supervisão' else  rsu.rsudsc end as realizadopor,
					oi.obrpercexec as executado,
					oi.obrvlrrealobra as vlr
				FROM obras.obrasempreendimento emp
				inner join obras.obrainfraestrutura oi on oi.obrid = emp.obrid
				INNER JOIN entidade.entidade ee ON ee.entid = oi.entidunidade
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				inner JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				LEFT JOIN 
					( SELECT oar.arqid as arqfoto, obrid FROM obras.arquivosobra oar 
                     INNER JOIN
                     	public.arquivo arq ON arq.arqid = oar.arqid
                     WHERE
                     	aqostatus = 'A'
                        and tpaid = 21
                        and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
                    order by oar.arqid limit 1
					) af ON af.obrid = oi.obrid
				LEFT JOIN
					( SELECT rstoid as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' order by rstoid desc limit 1  ) re ON re.obrid = oi.obrid
				LEFT JOIN
					(SELECT
						supvid as supervisao, rsuid,obrid, supdtinclusao as supdtinclusao
					FROM
						obras.supervisao s
					WHERE
						supvid = ( select supvid from obras.supervisao ss where ss.obrid = s.obrid and ss.supstatus = 'A' order by ss.supvdt desc, ss.supvid desc limit 1) ) AS ov ON ov.obrid = oi.obrid
				LEFT JOIN
					obras.supervisao sup on sup.supvid = ov.supervisao
				LEFT JOIN
					obras.realizacaosupervisao rsu on rsu.rsuid = sup.rsuid
				WHERE
					--oi.obrid in (select emp.obrid from obras.obrasempreendimento emp where emp.empid = $empid and emp.obestatus = 'A') AND
					emp.empid = $empid and
					oi.obsstatus = 'A' and
					obestatus = 'A'
					".($arrWhere ? " and ".implode(" and ", $arrWhere) : "")."
				GROUP BY
					af.arqfoto,
					oi.obrpercexec,
					re.restricao,
					oi.obridaditivo,oi.obrid,oi.obrdesc,oi.obrdtinicio,oi.obrdttermino,oi.obrdtvistoria,oi.obsdtinclusao,oi.stoid,
					ee.entnome,tm.mundescricao,ed.estuf,
					so.stodesc,
					oi.numconvenio,
					oi.obrvalorprevisto,
					ov.supervisao,
					oi.obrcustocontrato,
					oi.obrvlrrealobra,
					oi.obrpercexec,
					oi.preid,
					ov.supdtinclusao,
					rsu.rsudsc";
	//dbg(simec_htmlentities($sql),1);
	$arrCabecalho = array("F","R","ID","ID Pré-Obra","Convênio","Nome da Obra","Unidade Implantadora","Município/UF","Data de Início","Situação da Obra","Última Atualização","Última Vistoria","Realizado por","% Executado","Valor da Obra");
	$db->monta_lista($sql,$arrCabecalho,100,10,"N","95%");
}

function pesquisarEmpreendimento()
{
	return $_POST;
}

function excluirEmpreendimento()
{
	global $db;
	extract($_POST);
	if($empid){
		$sql = "update obras.obrasempreendimento set obestatus = 'I' where empid = $empid;
				update obras.empreendimento set empstatus = 'I' where empid = $empid;";
		$db->executar($sql);
		$db->commit();
		$db->sucesso("principal/empreendimento");
	}else{
		echo "<script>alert('Não foi possível excluir o empreendimento');window.location='obras.php?modulo=principal/empreendimento&acao=A'</script>";
	}
	exit;	
}

if( $_SESSION["obrasbkp"]["orgid"] ){
	$_SESSION["obras"]["filtros"] = $_SESSION["obrasbkp"]["filtrosbkp"];
	$_SESSION["obras"]["orgid"]   = $_SESSION["obrasbkp"]["orgid"];
	$_SESSION["obras"]["ordem"]   = $_SESSION["obrasbkp"]["ordem"];
	$_SESSION["obras"]["lista"]   = $_SESSION["obrasbkp"]["lista"];
	
	unset($_SESSION["obrasbkp"]);
}

// cria a sessão com o tipo de ensino selecionado
$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"];
if ( !possuiPerfil( PERFIL_ADMINISTRADOR ) ) {
	$dados = obrPegaOrgidPermitido( $_SESSION["usucpf"] );
	$i=0;
	if ( is_array($dados) && $dados[0] ){
		foreach( $dados as $orgao ){
			$orgid[$i] = $orgao['id'];
			$i++;
		}
		if ( !in_array($_SESSION["obras"]["orgid"],$orgid) ){
			$_SESSION["obras"]["orgid"] = $orgid[0];
		}
	}
}
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";
echo obrMontaAbasTipoEnsino( $_SESSION["usucpf"], $_SESSION["obras"]["orgid"] );
$titulo_modulo = "Empreendimento";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

//REGRA DE HOSPITAIS
if($_SESSION["obras"]["orgid"]==5){
	$arrCampo = "upper(ena.entsig) || ' - ' || upper(ent.entnome) as descricao";
	$arrInnerJoin = "INNER JOIN
						entidade.funcaoentidade fen ON fen.entid = ent.entid 
					INNER JOIN
						entidade.funcao fun ON fun.funid = fen.funid
					INNER JOIN
						entidade.funentassoc fue ON fue.fueid = fen.fueid
					INNER JOIN
						entidade.entidade ena ON ena.entid = fue.entid ";
}else{
	$arrCampo = "upper(ent.entnome) as descricao";
	$arrInnerJoin = "";
}

$arrPerfis = pegaPerfilGeral();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	jQuery.noConflict();
	function verTodosEmpreendimento()
	{
		window.location.href = window.location;
	}
	function pesquisarEmpreendimento()
	{
		jQuery("#formulario").submit();
	}
	function addEmpreendimento()
	{
		window.location.href = "obras.php?modulo=principal/cadastroEmpreendimento&acao=A";
	}
	function alterarEmpreendimento(empid)
	{
		jQuery("[name='empid']").val(empid);
		jQuery("#formulario").attr("action","obras.php?modulo=principal/cadastroEmpreendimento&acao=A");
		jQuery("[name='requisicao']").val("alterarEmpreendimento");
		jQuery("#formulario").submit();
	}
	function excluirEmpreendimento(empid)
	{
		if(confirm("Deseja realmente excluir o Empreendimento?")){
			jQuery("[name='empid']").val(empid);
			jQuery("[name='requisicao']").val("excluirEmpreendimento");
			jQuery("#formulario").submit();
		}
	}
	function exibirObrasEmpreendimento(empid)
	{
		jQuery('#img_mais_' + empid).hide();
		jQuery('#img_menos_' + empid).show();
		var filtro = jQuery("#formulario").serialize();
		jQuery("#filtro_anterior").val(filtro);
		if(!jQuery('#linha_' + empid).html() || filtro != jQuery("#filtro_anterior").val()){
			divCarregando();
			jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=exibirObrasEmpreendimento&" + filtro + "&empid=" + empid,
			   success: function(msg){
			   		jQuery('#tr_' + empid).show();
			   		jQuery('#linha_' + empid).html( msg );
			   		divCarregado();
			   }
			 });
		}else{
			jQuery('#tr_' + empid).show();
			jQuery('#linha_' + empid).html( msg );
		}
	}
	function esconderObrasEmpreendimento(empid)
	{
		jQuery('#img_mais_' + empid).show();
		jQuery('#img_menos_' + empid).hide();
		jQuery('#tr_' + empid).hide();
	}
</script>
<form id="formulario" name="formulario" method="post">
	 <input type="hidden" name="orgig" value="<?php echo $_SESSION["obras"]["orgid"] ?>"  />
	 <input type="hidden" name="empid" value=""  />
	 <input type="hidden" name="filtro_anterior" id="filtro_anterior"  value=""  />
	 <input type="hidden" name="requisicao" value="pesquisarEmpreendimento"  />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Nome do Empreendimento:</td>
			<td><?php echo campo_texto("empnome","N","S","",60,255,"","","","",""," id='empnome' ") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome da Obra / Nº do Convênio / Nº do PI / ID:</td>
			<td>
				<?php echo campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Unidade:</td>
			<td>
				<?php
					if(in_array(PERFIL_SUPERVISORUNIDADE,$arrPerfis)){
						$arrWhereUnidade[] = "ent.entid in (select ure.entid from obras.usuarioresponsabilidade ure where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PERFIL_SUPERVISORUNIDADE." and ure.entid is not null)";
					} 
					$sql = "SELECT 
								ent.entid as codigo, 
								$arrCampo 
							FROM
								entidade.entidade ent
							INNER JOIN 
								obras.obrainfraestrutura oi ON oi.entidunidade = ent.entid 
								$arrInnerJoin
							WHERE
								orgid = {$_SESSION["obras"]["orgid"]} AND
								obsstatus = 'A'
								".($arrWhereUnidade ? " and ".implode(" and ",$arrWhereUnidade) : "")."
							GROUP BY 
								codigo,
								descricao
							ORDER BY 
								descricao";
				
					$db->monta_combo( "entid", $sql, "S", "Todas", "", "", "", "", "N", "entid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >UF:</td>
			<td>
				<?php 
					$sql = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado
							ORDER BY
								estdescricao";
				
					$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td>
				<?php echo campo_texto( 'mundsc', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" ><img style="cursor:pointer;vertical-align:bottom" src="../imagens/gif_inclui.gif" onclick="addEmpreendimento()" /> <span  style="cursor:pointer" onclick="addEmpreendimento()" >Cadastrar Empreendimento</span></td>
			<td class="SubTituloEsquerda" >
				<input type="button" name="btn_buscar" value="Buscar" onclick="pesquisarEmpreendimento()" />
				<input type="button" name="btn_ver_todos" value="Ver Todos" onclick="verTodosEmpreendimento()" />
			</td>
		</tr>
	</table>
</form>
<?php
if($_POST['empnome']){
	$arrWhere[] = "removeacento(upper(empnome)) like removeacento(upper('%{$_POST['empnome']}%'))";
}
if($_POST['entid']){
	$arrWhere[] = "ent.entid = {$_POST['entid']}";
}
if($_POST['obrtextobusca']){	
	$filtro = "( removeacento(upper(obr.obrdesc)) ilike removeacento(upper('%{$_POST["obrtextobusca"]}%')) OR ";
	$filtro .= " upper(ee.entnome) ilike upper('%{$_POST["obrtextobusca"]}%') OR ";
	$filtro .= " upper(mpi.plicod) ilike upper('%{$_POST["obrtextobusca"]}%') OR ";
	$filtro .= " obr.obrid::text ilike ('%".(int)$_POST["obrtextobusca"]."%') )";
	$arrWhere[] = $filtro;
	
}
if($_POST['mundsc']){	
	$filtro = "removeacento(upper(tm.mundescricao)) ilike removeacento(upper('%{$_POST["mundsc"]}%'))";
	$arrWhere[] = $filtro;
	
}
if($_POST['estuf']){	
	$filtro = "tm.estuf = '{$_POST["estuf"]}'";
	$arrWhere[] = $filtro;
	
}
if(in_array(PERFIL_SUPERVISORUNIDADE,$arrPerfis)){
	$arrWhere[] = "ent.entid in (select ure.entid from obras.usuarioresponsabilidade ure where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PERFIL_SUPERVISORUNIDADE." and ure.entid is not null)";
}

if(in_array(PERFIL_SUPERUSUARIO,$arrPerfis)){
	$colunaAcao = "CASE WHEN count(distinct obe.obrid) > 0
						THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir empreendimentos com obras associadas.\')\" src=\"../imagens/excluir_01.gif\" />'
						ELSE '<img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"excluirEmpreendimento(' || emp.empid || ')\" src=\"../imagens/excluir.gif\" />'
					END";
}elseif(in_array(PERFIL_ADMINISTRADOR,$arrPerfis) || in_array(PERFIL_SUPERVISORMEC,$arrPerfis)){
	$colunaAcao = "CASE 
						WHEN count(distinct obe.obrid) > 0 and emp.usucpf = '{$_SESSION['usucpf']}' --Não Pode excluir se houver obras e tiver criado
							THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir empreendimentos com obras associadas.\')\" src=\"../imagens/excluir_01.gif\" />'
						WHEN count(distinct obe.obrid) > 0 and emp.usucpf != '{$_SESSION['usucpf']}' --Não Pode excluir e alterar se ñ tiver criado
							THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para alterar este empreendimento.\')\" src=\"../imagens/alterar_01.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir empreendimentos com obras associadas.\')\" src=\"../imagens/excluir_01.gif\" />'
						WHEN count(distinct obe.obrid) = 0 and emp.usucpf = '{$_SESSION['usucpf']}' --Pode excluir se ñ houver obras e tiver criado
							THEN '<img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"excluirEmpreendimento(' || emp.empid || ')\" src=\"../imagens/excluir.gif\" />'
						ELSE '<img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para alterar este empreendimento.\')\" src=\"../imagens/alterar_01.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para excluir este empreendimento.\')\" src=\"../imagens/excluir_01.gif\" />'
					END";
}elseif(in_array(PERFIL_SUPERVISORUNIDADE,$arrPerfis)){
	$colunaAcao = "CASE 
						WHEN count(distinct obe.obrid) > 0 and emp.usucpf = '{$_SESSION['usucpf']}' --Não Pode excluir se houver obras e tiver criado
							THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir empreendimentos com obras associadas.\')\" src=\"../imagens/excluir_01.gif\" />'
						WHEN count(distinct obe.obrid) > 0 and emp.usucpf != '{$_SESSION['usucpf']}' --Não Pode excluir e alterar se ñ tiver criado
							THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para alterar este empreendimento.\')\" src=\"../imagens/alterar_01.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir empreendimentos com obras associadas.\')\" src=\"../imagens/excluir_01.gif\" />'
						WHEN count(distinct obe.obrid) = 0 and emp.usucpf = '{$_SESSION['usucpf']}' --Pode excluir se ñ houver obras e tiver criado
							THEN '<img style=\"cursor:pointer\" onclick=\"alterarEmpreendimento(' || emp.empid || ')\" src=\"../imagens/alterar.gif\" /> <img style=\"cursor:pointer\" onclick=\"excluirEmpreendimento(' || emp.empid || ')\" src=\"../imagens/excluir.gif\" />'
						ELSE '<img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para alterar este empreendimento.\')\" src=\"../imagens/alterar_01.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para excluir este empreendimento.\')\" src=\"../imagens/excluir_01.gif\" />'
					END";
}else{
	$colunaAcao = "CASE WHEN count(distinct obe.obrid) > 0
						THEN '<img style=\"cursor:pointer;display:none\" id=\"img_menos_' || emp.empid || '\" onclick=\"esconderObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/menos.gif\" /> <img style=\"cursor:pointer\" id=\"img_mais_' || emp.empid || '\" onclick=\"exibirObrasEmpreendimento(' || emp.empid || ')\" src=\"../imagens/mais.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para alterar este empreendimento.\')\" src=\"../imagens/alterar_01.gif\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Você não possui permissão para excluir este empreendimento.\')\" src=\"../imagens/excluir_01.gif\" />'
						ELSE ''
					END";
}

$sql = "select
			'<div style=\"text-align:right\" >' || $colunaAcao || '</div>' as acao,
			empnome,
			$arrCampo,
			to_char(empdtinclusao,'DD/MM/YYYY') as data_inclusao,
			upper(usunome) as responsavel,
			count(distinct obe.obrid) as total,
			'</tr><tr style=\"display:none\" id=\"tr_' || emp.empid || '\" ><td colspan=6 id=\"linha_' || emp.empid || '\" ></td></tr>' as linha
		from
			obras.empreendimento emp
		inner join
			seguranca.usuario usu ON usu.usucpf = emp.usucpf
		inner join
			entidade.entidade ent ON ent.entid = emp.entid
		left join
			obras.obrasempreendimento obe ON obe.empid = emp.empid and obestatus = 'A'
		left join
			obras.obrainfraestrutura obr ON obr.obrid = obe.obrid and obr.orgid = {$_SESSION["obras"]["orgid"]}
		left JOIN
			entidade.entidade ee ON ee.entid = obr.entidunidade
		left JOIN
			entidade.endereco ed ON ed.endid = obr.endid
		left JOIN
			territorios.municipio tm ON tm.muncod = ed.muncod
		LEFT JOIN
			( SELECT max(mpi.pliid) as pi, a.obrid FROM monitora.pi_obra a INNER JOIN monitora.pi_planointerno mpi ON mpi.pliid = a.pliid WHERE mpi.plistatus = 'A' GROUP BY a.obrid ) o ON o.obrid = obr.obrid
		LEFT JOIN
			monitora.pi_planointerno mpi ON mpi.pliid = o.pi AND mpi.plistatus = 'A'
		$arrInnerJoin
		where
			empstatus = 'A' AND emp.orgid = {$_SESSION["obras"]["orgid"]}
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
		group by
			emp.empnome,descricao,emp.empid,emp.empdtinclusao,emp.usucpf,usu.usunome";
//dbg(simec_htmlentities($sql),1);
$arrCabecalho = array("Ação","Empreendimento","Unidade","Data de Inclusão","Responsável","Total de Obras");
$db->monta_lista($sql,$arrCabecalho,100,10,"N","95%");
?>