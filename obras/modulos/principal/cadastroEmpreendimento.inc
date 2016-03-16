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

function alterarEmpreendimento()
{
	global $db;
	extract($_POST);
	$sql = "select * from obras.empreendimento where empid = $empid and empstatus = 'A';";
	return $db->pegaLinha($sql);
}

function salvarEmpreendimento()
{
	global $db;
	extract($_POST);
	if($empid){
		$sqlI = "update obras.obrasempreendimento set obestatus = 'I' where empid = $empid;
				 update obras.empreendimento set empnome = '$empnome', entid = $entid where empid = $empid;";
		$arr_obrid = !$arr_obrid ? array() : $arr_obrid;
		foreach($arr_obrid as $o){
			if($o){
				$sqlI.="insert into obras.obrasempreendimento (empid,obrid,obestatus,obedtinclusao) values ($empid,$o,'A',now());";
			}
		}
	}else{
		$sql = "insert into obras.empreendimento (entid,orgid,empdtinclusao,empstatus,empnome,usucpf) values ($entid,$orgig,now(),'A','$empnome','{$_SESSION['usucpf']}') returning empid;";
		$empid = $db->pegaUm($sql);
		$arr_obrid = !$arr_obrid ? array() : $arr_obrid;
		foreach($arr_obrid as $o){
			if($o){
				$sqlI.="insert into obras.obrasempreendimento (empid,obrid,obestatus,obedtinclusao) values ($empid,$o,'A',now());";
			}
		}
	}
	$db->executar($sqlI);
	$db->commit();
	$db->sucesso("principal/empreendimento");
	exit;
}

function filtraObrasUnidade()
{
	global $db;
	extract($_POST);
	if($entid){ 
		$Sql = "SELECT 
			   		obrid AS codigo, 
			   		upper(obrdesc) AS descricao 
			   FROM 
			   		obras.obrainfraestrutura
			   where
			   		obsstatus = 'A'
			   	and
			   		entidunidade = $entid
			   	and
					orgid = {$_SESSION["obras"]["orgid"]}
			   order by
					descricao";
		if($empid){
			$SqlCarregado = "  SELECT 
							   		obr.obrid AS codigo, 
							   		upper(obrdesc) AS descricao 
							   FROM 
							   		obras.obrasempreendimento emp
							   	inner join 
							   		obras.obrainfraestrutura obr ON emp.obrid = obr.obrid
							   where
							   		obsstatus = 'A'
							   	and
							   		emp.empid = $empid
							   	and
							   		emp.obestatus = 'A'
							   order by
									descricao";

			$arr_obrid = $db->carregar($SqlCarregado);
		}
		combo_popup( 'arr_obrid', $Sql, 'Selecione a(s) obra(s) ', '360x460', 0, array(), "", "S", true, false, 5, 400,"","","",array(
			  array(
			  		"codigo" 	=> "obrdesc",
			  		"descricao" => "Obra"
			  		)
			  )	,"",true,true);
	}else{
		echo "Selecione uma Unidade.";
	}	
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

/* 
Corrigindo uma falta de padronização, 
em alguns trechos do código utiliza-se $_SESSION["obras"]["orgid"] e em outros $_SESSION["obra"]["orgid"]
*/
if(!$_SESSION["obras"]["orgid"] && $_SESSION["obra"]["orgid"]) $_SESSION["obras"]["orgid"] = $_SESSION["obra"]["orgid"];


if ( !possuiPerfil( PERFIL_ADMINISTRADOR ) ) {
	$dados = obrPegaOrgidPermitido( $_SESSION["usucpf"] );
	$i=0;
	if ( is_array($dados) && $dados[0] ){
		foreach( $dados as $orgao ){
			$orgid[$i] = $orgao['id'];
			$i++;
		}
		if(!is_array($orgid)) $orgid = array();
		if ( !in_array($_SESSION["obras"]["orgid"],$orgid) ){
			$_SESSION["obras"]["orgid"] = $orgid[0];
		}
	}
}

if($_SESSION["obras"]["orgid"] && $_SESSION["obra"]["orgid"]) {
	echo "<script>alert('Não foi identificado nenhum Tipo de ensino');window.location='obras.php?modulo=inicio&acao=A';</script>";
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

echo obrMontaAbasTipoEnsino( $_SESSION["usucpf"], $_SESSION["obras"]["orgid"] );
$titulo_modulo = "Empreendimento";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");
$arrPerfis = pegaPerfilGeral();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	jQuery.noConflict();
	function salvarEmpreendimento()
	{
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		if(erro == 0){
			selectAllOptions( document.getElementById( 'arr_obrid' ) );
			jQuery("#formulario").submit();
		}
	}
	function voltarEmpreendimento()
	{
		window.location.href = "obras.php?modulo=principal/empreendimento&acao=A";
	}
	function filtraObrasUnidade(entid)
	{
		var empid = jQuery("[name='empid']").val(); 
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=filtraObrasUnidade&entid=" + entid + "&empid=" + empid,
		   success: function(msg){
		   		jQuery('#td_obras').html( msg );
		   }
		 });
	}
</script>
<form id="formulario" name="formulario" method="post">
	<input type="hidden" name="requisicao" value="salvarEmpreendimento" />
	<input type="hidden" name="orgig" value="<?php echo $_SESSION["obras"]["orgid"] ?>"  />
	<input type="hidden" name="empid" value="<?php echo $empid ?>"  /> 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Nome do Empreendimento:</td>
			<td><?php echo campo_texto("empnome","S","S","",60,255,"","","","",""," id='empnome' ") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Unidade:</td>
			<td>
				<?php
					if(!$arrPerfis) $arrPerfis = array();
					if(in_array(PERFIL_SUPERVISORUNIDADE,$arrPerfis)){
						$arrWhereUnidade[] = "ee.entid in (select ure.entid from obras.usuarioresponsabilidade ure where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PERFIL_SUPERVISORUNIDADE." and ure.entid is not null)";
					}
					if($_SESSION["obras"]["orgid"]==5){
						$sql = "SELECT 
									ent.entid as codigo, 
									upper(ena.entsig) || ' - ' || upper(ent.entnome) as descricao
								FROM
									entidade.entidade ent
								INNER JOIN 
									obras.obrainfraestrutura oi ON oi.entidunidade = ent.entid
								INNER JOIN
									entidade.funcaoentidade fen ON fen.entid = ent.entid 
								INNER JOIN
									entidade.funcao fun ON fun.funid = fen.funid
								INNER JOIN
									entidade.funentassoc fue ON fue.fueid = fen.fueid
								INNER JOIN
									entidade.entidade ena ON ena.entid = fue.entid 
								WHERE
									orgid = {$_SESSION["obras"]["orgid"]} AND
									obsstatus = 'A'
									".($arrWhereUnidade ? " and ".implode(" and ",$arrWhereUnidade) : "")."
								GROUP BY 
									codigo, 
									descricao 
								ORDER BY 
									descricao";
					}else{
						$sql = "SELECT 
									ee.entid as codigo, 
									upper(ee.entnome) as descricao 
								FROM
									entidade.entidade ee
								INNER JOIN 
									obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
								WHERE
									orgid = {$_SESSION["obras"]["orgid"]} AND
									obsstatus = 'A'
									".($arrWhereUnidade ? " and ".implode(" and ",$arrWhereUnidade) : "")."
								GROUP BY 
									ee.entnome, 
									ee.entid 
								ORDER BY 
									ee.entnome";
					}
				
					$db->monta_combo( "entid", $sql, "S", "Selecione...", "filtraObrasUnidade", "", "", "", "S", "entid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Selecione a(s) obra(s):</td>
			<td id="td_obras" >
			<?php
			if($entid){ 
				$Sql = "SELECT 
					   		obrid AS codigo, 
					   		upper(obrdesc) AS descricao 
					   FROM 
					   		obras.obrainfraestrutura
					   where
					   		obsstatus = 'A'
					   	and
					   		entidunidade = $entid
					   	and
					   		orgid = {$_SESSION["obras"]["orgid"]}
					   order by
							descricao";
				if($empid){
					$SqlCarregado = "  SELECT 
									   		obr.obrid AS codigo, 
									   		upper(obrdesc) AS descricao 
									   FROM 
									   		obras.obrasempreendimento emp
									   	inner join 
									   		obras.obrainfraestrutura obr ON emp.obrid = obr.obrid
									   where
									   		obsstatus = 'A'
									   	and
									   		emp.empid = $empid
									   	and
									   		emp.obestatus = 'A'
									   order by
											descricao";

					$arr_obrid = $db->carregar($SqlCarregado);
				}
				combo_popup( 'arr_obrid', $Sql, 'Selecione a(s) obra(s) ', '360x460', 0, array(), "", "S", true, false, 5, 400,"","","",array(
					  array(
					  		"codigo" 	=> "obrdesc",
					  		"descricao" => "Obra"
					  		)
					  )	,"",true,true);
			}else{
				echo "Selecione uma Unidade.";
			}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" ></td>
			<td class="SubTituloEsquerda" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarEmpreendimento()" />
				<input type="button" name="btn_voltar" value="Voltar" onclick="voltarEmpreendimento()" />
			</td>
		</tr>
	</table>
</form>