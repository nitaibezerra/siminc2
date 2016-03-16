<?php

/*
 * Verifica se o usuário possui o perfil 'Gestor da Unidade de Planejamento' 
 * e possui a unidade do FNDE(26298) associada.
 */
$GESTOR_FNDE = false;

$sql = "SELECT
			trim(ur.unicod) as unicod
		FROM
			monitora.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION["usucpf"]."' AND
			ur.pflcod = 112 AND
			ur.rpustatus = 'A'";

$unidades = $db->carregar($sql);
if($unidades) {
	for($i=0; $i<count($unidades); $i++) {
		if($unidades[$i]["unicod"] == "26298") {
			$GESTOR_FNDE = true;
		}
	}
}


/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número
 */
define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);

/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número 
 */


// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

/*
 * SCRIPT que controle as regras de combobox
 */
include APPRAIZ."/includes/controlecombobox.inc";
if(is_array($_REQUEST['dados'])) {
	$parametroconstrutor = array_merge($_REQUEST['dados'], array());
}
$combo = new ControleComboBox($parametroconstrutor);

/*
 * Constantes das opções da combobox
 */
define('COMBO_REGRAGERAL', 5);
define('COMBO_REGRAPDEEPPA', 3);
/*
 * Constantes das opções da combobox
 */


/*
 * FIM SCRIPT que controle as regras de combobox
 */

// carrega as funções de integração
include "planotrabalho/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";
include "planotrabalho/_componentes.php";

// definindo variaveis
$atiid  = $_POST['atiid'] ? $_POST['atiid'] : $_GET['atiid']; 
$plicod = $_POST['plicod'] ? $_POST['plicod'] : $_GET['plicod']; 

// verifica se ocorre algum evento
if(isset($_REQUEST['evento']) && ($_REQUEST['evento'] != '') ){
	switch($_REQUEST['evento']) {
		// atualizar os dados do plano interno
		case "G":
		$sql = "UPDATE monitora.subacao SET sbatitulo='".$_POST['sbatitulo']."',
											sbasigla='".$_POST['sbasigla']."', 
											   sbadsc='".$_POST['sbadsc']."', 
       		 								   usucpf='".$_SESSION['usucpf']."',
       		 								   sbaobra=".$_REQUEST["sbaobra"].", 
       		 								   sbadata=NOW() 
       		    WHERE sbaid=".$_POST['sbaid'].";";
		$db->executar($sql);
		// atualizando os títulos dos plano internos que não foram cadastrados no SIAFI
		$sql = "UPDATE monitora.planointerno SET plititulo = substr(replace(plititulo, '".$_POST['sbatituloantigo']."', '".$_POST['sbatitulo']."'),1,45) WHERE sbaid='".$_POST['sbaid']."' AND plisituacao!='S'";
		$db->executar($sql);
		
		$db->executar("DELETE FROM monitora.subacaounidade WHERE sbaid='".$_POST['sbaid']."'");
		$db->executar("DELETE FROM monitora.subacaodotacao WHERE sbaid='".$_POST['sbaid']."'");
		$db->executar("DELETE FROM monitora.subacaodados WHERE sbaid=".$_POST['sbaid']."");
		
		if($_POST['entid'][0]) {
			foreach($_POST['entid'] as $entid) {
				$sql = "INSERT INTO monitora.subacaounidade(sbaid, entid)
    					VALUES ('".$_POST['sbaid']."', '".$entid."');";
				$db->executar($sql);
			}
		}

		if(is_array($_POST['ptrid'])) {
			//$dataInsercao = date("Y-m-d H:i:s");
			
			foreach($_POST['ptrid'] as $ptrid ) {
				$valor = $_POST['valor'][$ptrid] ? $_POST['valor'][$ptrid] : 'null';		
				$valor = str_replace(array(".",","), array("","."), $valor);
				
				$sql = "INSERT INTO monitora.subacaodotacao 
					  		(sbaid, ptrid, sadvalor) 
	     			  	  VALUES ('".$_POST['sbaid']."',".$ptrid.",".$valor.")
	     			   ";	
				$db->pegaUm($sql);
			}
		}		

		// insere o nivel/etapa
		if($_POST['niveletapa'][0]) {
			foreach($_POST['niveletapa'] as $niveletapa) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$_POST['sbaid'].", ".$niveletapa.");";
				$db->executar($sql);
			}
		}
		
		// insere a categoria
		if($_POST['categoria'][0]) {
			foreach($_POST['categoria'] as $categoria) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$_POST['sbaid'].", ".$categoria.");";
				$db->executar($sql);
			}
		}
		
		// insere a modalidade
		if($_POST['modalidade'][0]) {
			foreach($_POST['modalidade'] as $modalidade) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$_POST['sbaid'].", ".$modalidade.");";
				$db->executar($sql);
			}
		}
		
		$db->commit();
		//echo "<form method='POST' name='formulario'><input type='hidden' name='evento' value='A'><input type='hidden' name='sbaid' value='".$_POST['sbaid']."'></form>
		//	  <script>alert('Dados gravados com sucesso.');document.formulario.submit();</script>";
		echo "<script>alert('Dados gravados com sucesso.'); location.href='monitora.php?modulo=principal/planotrabalho/programainstitucional&acao=A&evento=A&sbaid=".$_POST['sbaid']."';</script>";
		exit;
		
		
		// carregar os dados do pi
		case 'A':
		
		// pegar os dados comuns dos PI
		$subacao = $db->pegaLinha("SELECT * FROM monitora.subacao WHERE sbaid = '".$_REQUEST['sbaid']."'");
		/*
		 * Verifica se existe programa  
		 */
		if($subacao) {
			
			$sbatitulo 	   = $subacao['sbatitulo'];
			$sbasigla 	   = $subacao['sbasigla'];
			$sbadsc 	   = $subacao['sbadsc'];
			$sbacod 	   = $subacao['sbacod'];
			$sbaobra	   = $subacao['sbaobra'];
			//$eqdid	 	   = $subacao['eqdid'];
			$exedsc 	   = $db->pegaUm("SELECT cdtcod ||' - '|| cdtdsc FROM public.combodadostabela WHERE cdtid='".$subacao['exeid']."'");
			$gstdsc 	   = $db->pegaUm("SELECT cdtcod ||' - '|| cdtdsc FROM public.combodadostabela WHERE cdtid='".$subacao['gstid']."'");
			$eqddsc 	   = $db->carregar("SELECT cdtcod ||' - '|| cdtdsc as enquadramento FROM public.combodadostabela cd 
											LEFT JOIN monitora.subacaoenquadramento se ON se.eqdid=cd.cdtid WHERE sbaid='".$subacao['sbaid']."'");
			
			$entid = $db->carregar("SELECT ent.entid as codigo,
								 			CASE fen.funid
										   	when 34 then 
										   		ent.entungcod ||' - '|| ent.entnome 
										   	else 
										   		ent.entunicod ||' - '|| ent.entnome 
										   	end as descricao
											--ent.entunicod||' - '||ent.entnome as descricao 
									FROM monitora.subacaounidade ppe 
									LEFT JOIN entidade.entidade ent On ent.entid = ppe.entid 
									LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid  
									WHERE ppe.sbaid = '".$subacao['sbaid']."'");
			
			// carrega os dados do nível/etapa
			$niveletapa = $db->carregar("SELECT
											cdt.cdtid AS codigo, 
											cdt.cdtcod || ' - ' || cdt.cdtdsc AS descricao
			    					 	 FROM 	
			    					 		public.combodadostabela cdt
			    					 	 INNER JOIN
			    					 	 	monitora.subacaodados sad ON sad.sbaid = ".$subacao['sbaid']." AND
			    					 	 								 sad.cdtid = cdt.cdtid
				    					 WHERE
				    					 	cdt.ctbid = 6 AND
				    					 	cdt.cdtstatus = 'A'");
			
			// carrega os dados da categoria
			$categoria = $db->carregar("SELECT
											cdt.cdtid AS codigo, 
											cdt.cdtcod || ' - ' || cdt.cdtdsc AS descricao
			    					 	 FROM 	
			    					 		public.combodadostabela cdt
			    					 	 INNER JOIN
			    					 	 	monitora.subacaodados sad ON sad.sbaid = ".$subacao['sbaid']." AND
			    					 	 								 sad.cdtid = cdt.cdtid
				    					 WHERE
				    					 	cdt.ctbid = 7 AND
				    					 	cdt.cdtstatus = 'A'");
			
			// carrega os dados da modalidade
			$modalidade = $db->carregar("SELECT
											cdt.cdtid AS codigo, 
											cdt.cdtcod || ' - ' || cdt.cdtdsc AS descricao
			    					 	 FROM 	
			    					 		public.combodadostabela cdt
			    					 	 INNER JOIN
			    					 	 	monitora.subacaodados sad ON sad.sbaid = ".$subacao['sbaid']." AND
			    					 	 								 sad.cdtid = cdt.cdtid
				    					 WHERE
				    					 	cdt.ctbid = 8 AND
				    					 	cdt.cdtstatus = 'A'");
			
			$sql = "SELECT
									dtl.ptrid,
									dtl.ptres,
									trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
									ent.entnome,
									SUM(dtl.ptrdotacao) as dotacaoinicial,
									'' || coalesce(cast(SUM(dt.valor) as varchar),'0.00') || '' as detalhamento,
									(SUM(dtl.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca,	
									coalesce(rofautorizado, 0.00) as rofautorizado ,
									coalesce(rof_credrecebidodireta, 0.00) as rof_credrecebidodireta ,
									coalesce(rof_credrecebidoindireta, 0.00) as rof_credrecebidoindireta
								  FROM monitora.acao aca 
									inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid 
									inner JOIN entidade.entidade ent ON ent.entunicod = dtl.unicod AND ent.entungcod IS NULL					  
									LEFT JOIN ( select ptrid,sbaid,
												sum( sadvalor ) as valor
											from  monitora.subacaodotacao
											group by ptrid,sbaid ) dt ON dtl.ptrid = dt.ptrid
									LEFT JOIN ( select 
													ptrid,
													e.funcod,
													e.sfucod,
													e.prgcod, 	
													e.acacod,
													e.unicod,
													e.loccod,
													sum(coalesce(rofdot_ini ,0.00) +  coalesce(rofpl_credadicionais ,0.00)) as rofautorizado ,
													sum(coalesce(rof_credrecebidodireta ,0.00)) as rof_credrecebidodireta ,
													sum(coalesce(rof_credrecebidoindireta ,0.00)) as rof_credrecebidoindireta  
												from 
												financeiro.execucao e 
												inner join monitora.ptres p ON p.ptres = e.ptres
												where p.ptrano = '".$_SESSION['exercicio']."' and e.prgcod <> '' and rofano = '".$_SESSION['exercicio']."' and ( rof_credrecebidodireta <> 0 or rof_credrecebidoindireta <> 0 )
												group by 										ptrid,
													e.funcod,
													e.sfucod,
													e.prgcod, 	
													e.acacod,
													e.unicod,
													e.loccod
												 ) cred ON cred.ptrid = dtl.ptrid
									WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
								  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."')
								  	and dt.sbaid = ".$subacao['sbaid']."
								  	GROUP BY dtl.ptrid,dtl.ptres,descricao,ent.entnome, rofautorizado, rof_credrecebidodireta, rof_credrecebidoindireta
								  	ORDER BY 1;
								  ";
			$ptresacao = $db->carregar($sql);
			
			
		} else {
			
			echo "<script>alert('SubAção não encontrado');window.location='?modulo=principal/planotrabalho/pesqSubacao&acao=A';</script>";
			exit;
			
		}
		break;
		
		
		// inserir plano interno
		case 'I':
			
		$valor = explode("||",$_POST['campo3']);
		$exeid = $valor[0];
		$gerador1 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");

		$valor = explode("||",$_POST['campo4']);
		$gstid = $valor[0];
		$gerador2 = $db->pegaUm("SELECT cdtcod FROM public.combodadostabela WHERE cdtid='".$valor[0]."'");
		
		/*** LÓGICA PARA GERAR O SEQUENCIAL ***/
		// Maneira antiga de recuperar o sequencial.
		//$gerador3 = controlegeradorsequencialpi($gerador1.$gerador2,2);
		
		// Nova maneira.
		$identificador = $gerador1.$gerador2;
		$seq = $db->pegaUm("SELECT gspseq FROM public.geradorsequencialpi WHERE gspidentificador = '".$identificador."' ORDER BY gspid DESC");
		
		if($seq) {
			$gspseq = retornaseq(substr($seq, -2));
			$gspseq = str_pad($gspseq, 4, "0", STR_PAD_LEFT);
		} else {
			$gspseq = "0001";
		}
		
		$sql = "INSERT INTO geradorsequencialpi(gspseq, gspidentificador)
	    			VALUES ('".$gspseq."', '".$identificador."');";
		$db->executar($sql);
		$db->commit();
			
		// No caso da subação retorna apenas os dois últimos campos do código gerado.
		$gerador3 = substr($gspseq, -2);
		
		// Então gera o sbacod para gravar na tabela 'monitora.subacao'.
		$sbacod = $gerador1.$gerador2.$gerador3;
		
		$sql = "INSERT INTO monitora.subacao(
            		sbatitulo, sbasigla, sbacod, exeid, gstid, eqdid, sbadsc, sbastatus, usucpf, sbadata, sbaobra)
		    	 VALUES ('".$_POST['sbatitulo']."', '".$_POST['sbasigla']."', '".$sbacod."', '".$exeid."', '".$gstid."', 
		    	 		 NULL, '".$_POST['sbadsc']."', 'A', '".$_SESSION['usucpf']."', NOW(), ".$_POST['sbaobra'].") 
		    	 RETURNING sbaid;
		       ";
		$sbaid = $db->pegaUm($sql);
		
		if($_POST['eqdid'][0]) {
			foreach($_POST['eqdid'] as $eqdid) {
				$sql = "INSERT INTO monitora.subacaoenquadramento(sbaid, eqdid)
    					VALUES ('".$sbaid."', '".$eqdid."');";
				$db->executar($sql);
			}
		}
		
	
		if($_POST['entid'][0]) {
			foreach($_POST['entid'] as $entid) {
				$sql = "INSERT INTO monitora.subacaounidade(sbaid, entid)
    					VALUES ('".$sbaid."', '".$entid."');";
				$db->executar($sql);
			}
		}

		// insere o nivel/etapa
		if($_POST['niveletapa'][0]) {
			foreach($_POST['niveletapa'] as $niveletapa) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$sbaid.", ".$niveletapa.");";
				$db->executar($sql);
			}
		}
		
		// insere a categoria
		if($_POST['categoria'][0]) {
			foreach($_POST['categoria'] as $categoria) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$sbaid.", ".$categoria.");";
				$db->executar($sql);
			}
		}
		
		// insere a modalidade
		if($_POST['modalidade'][0]) {
			foreach($_POST['modalidade'] as $modalidade) {
				$sql = "INSERT INTO monitora.subacaodados(sbaid, cdtid)
    					VALUES (".$sbaid.", ".$modalidade.");";
				$db->executar($sql);
			}
		}
		
		if(is_array($_POST['ptrid'])) {
			//$dataInsercao = date("Y-m-d H:i:s");
			foreach($_POST['ptrid'] as $ptrid ) {
				
				$valor = $_POST['valor'][$ptrid] ? $_POST['valor'][$ptrid] : 'null';		
				$valor = str_replace(array(".",","), array("","."), $valor);
				
				$sql = "INSERT INTO monitora.subacaodotacao 
					  		(sbaid, ptrid, sadvalor) 
	     			  	  VALUES ('".$sbaid."',".$ptrid.",".$valor.")
	     			   ";	
				
				$db->pegaUm($sql);
			}
		}		
		
		$db->commit();
		echo "<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/planotrabalho/programainstitucional&acao=A';</script>";
		exit;
		
		
		case 'E':
		$pliid = $db->pegaUm("SELECT pliid FROM monitora.planointerno WHERE plistatus='A' and sbaid='".$_REQUEST['sbaid']."'");
		if($pliid) {
			
			echo "<script>alert('Não é possível excluir! \nExistem um ou mais planos internos associados a esta subação.');window.location = '?modulo=principal/planotrabalho/pesqSubacao&acao=A';</script>";
			exit;
			
		} else {
		
			$sbacod = $db->pegaUm("SELECT sbacod FROM monitora.subacao WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM public.geradorsequencialpi WHERE gspidentificador='".substr($sbacod,0,2)."' AND gspseq='00".substr($sbacod,2,2)."'");
			$db->executar("DELETE FROM monitora.subacaounidade WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM monitora.subacaodotacao WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM monitora.subacaodados WHERE sbaid=".$_REQUEST['sbaid']."");
			$sql_D = "DELETE FROM monitora.subacao where sbaid = '".$_REQUEST['sbaid']."'";
			$db->executar($sql_D);
			$db->commit();
			echo "<script>alert('SubAção removido com sucesso.');window.location = '?modulo=principal/planotrabalho/pesqSubacao&acao=A';</script>";
			exit;
			
			
		}
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Cadastro de Subação", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="<? echo (($_REQUEST['sbaid'])?"G":"I"); ?>">
<input type="hidden" name="sbaid"  id="sbaid" value="<?=$_REQUEST['sbaid'];?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td><? 
			echo campo_texto('sbatitulo','S','S','',30,25,'','',null,null,null,'',"limitaTitulo();");
			echo "<input type='hidden' name='sbatituloantigo' value='".$sbatitulo."'>"; 
		?>
		
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Título Abreviado / Sigla:</td>
    <td><? 
			echo campo_texto('sbasigla','S','S','',16,15,'','',null,null,null,''); 
		?>
		
	</td>
</tr>





<?
if($_REQUEST['sbaid']) {
	?>
	<tr>
		<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		<td>
		<?
		if($eqddsc[0]) {
			foreach($eqddsc as $eqd) {
				echo $eqd['enquadramento']."<br>";
			}
		} else {
			echo "Não definido";	
		}
		?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
	    <td><? echo $exedsc; ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Gestor da SubAção:</td>
	    <td><? echo $gstdsc; ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Código da Subação:</td>
	    <td><? echo $sbacod; ?></td>
	</tr>
	<?	
} else {
	//echo $combo->pegarComboHTML(COMBO_REGRAPDEEPPA);
	?>
	
		<tr>
			<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		    <td>
				<?
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 5";
				combo_popup( "eqdid", $sql, "Enquadramento da Despesa", "600x600", 0, array(), "", "S", false, false, 5, 400 );
				?>    	
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
		    <td>
				<?
					if($GESTOR_FNDE) {
						echo "F - FNDE";
						$sql = "SELECT cdtid FROM public.combodadostabela WHERE ctbid = 3 and trim(upper(cdtdsc)) = 'FNDE'";
						$campo3 = $db->pegaUm($sql);
						echo "<input type=\"hidden\" id=\"000000003_0\" name=\"campo3\" value=\"".$campo3."\">";
					} else {
						$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3";  
						$db->monta_combo('campo3', $sql, 'S', 'Selecione', '', '', '', '100', 'S', '000000003_0');
					}
				?>    	
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
		    <td>
		    	
				<?
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4";  
				$db->monta_combo('campo4', $sql, 'S', 'Selecione', '', '', '', '100', 'S', '000000003_1'); 
				?>    	
		    </td>
		</tr>
	<?
}
?>
		<tr id="tr_niveletapa" style="display:<?= ($GESTOR_FNDE) ? 'table-row' : 'none'; ?>;">
			<td align='right' class="SubTituloDireita">Nível/Etapa de Ensino:</td>
		    <td>
		    	<?
					$sqlNivelEtapa = "SELECT 
										cdtid AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
			    					  FROM 
			    					  	public.combodadostabela 
			    					  WHERE 
			    					  	ctbid = 6 AND 
			    					  	cdtstatus = 'A' 
			    					  ORDER BY
			    					  	cdtcod";

					combo_popup( "niveletapa", $sqlNivelEtapa, "Nível/Etapa de Ensino", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		    	?>
		    </td>
		</tr>
		
		<tr id="tr_categoria" style="display:<?= ($GESTOR_FNDE) ? 'table-row' : 'none'; ?>;">
			<td align='right' class="SubTituloDireita">Categoria de Apropriação:</td>
		    <td>
		    	<?
					$sqlCategoria = "SELECT 
										cdtid AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
			    					 FROM 
			    					 	public.combodadostabela 
			    					 WHERE
			    					 	ctbid = 7 AND
			    					 	cdtstatus = 'A' 
			    					 ORDER BY 
			    					 	cdtcod";

					combo_popup( "categoria", $sqlCategoria, "Categoria de Apropriação", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		    	?>
		    </td>
		</tr>
		
		<tr id="tr_modalidade" style="display:<?= ($GESTOR_FNDE) ? 'table-row' : 'none'; ?>;" >
			<td align='right' class="SubTituloDireita">Modalidade de Ensino/Tema/Público:</td>
		    <td>
		    	<?
					$sqlModalidade = "SELECT 
										cdtid AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
					    			  FROM 
					    			  	public.combodadostabela 
					    			  WHERE
					    			  	ctbid = 8 AND
					    			  	cdtstatus = 'A'
					    			  ORDER BY
					    			  	cdtcod";

					combo_popup( "modalidade", $sqlModalidade, "Modalidade de Ensino", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		    	?>
		    </td>
		</tr>
		
		<tr>
			<td align='right' class="SubTituloDireita">Permite Obras?</td>
		    <td>
				<input type="checkbox" id="checkPermiteObras" onchange="marcaPermiteObras();" <?= ($sbaobra == "t") ? 'checked="checked"' : '' ?>>
				<input type="hidden" id="sbaobra" name="sbaobra" value="<?= (isset($sbaobra) && $sbaobra=="t") ? "true" : "false" ?>">
		    </td>
		</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
	<td>
	<?php
	
	$sqlComboEntidade = "SELECT
					   ent.entid as codigo,
					   CASE fen.funid
					   	when 34 then 
					   		entungcod ||' - '|| entnome 
					   	else 
					   		entunicod ||' - '|| entnome 
					   	end as descricao
					   FROM entidade.entidade ent 
					   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					   WHERE entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') and fen.funid <> 33 and ent.entid NOT IN ('391293','391294') ORDER BY entunicod";

	combo_popup( "entid", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Ações:</td>
    <td >
        <table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
		<tr>
		<td style="background-color: #C9C9C9;" colspan="9" align="center"><b>Detalhamento Orçamentário</b></td>
		</tr>
        <tr>
			<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="ptrid"></td>
			<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação inicial</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado na SubAção</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Crédito Autorizado</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Crédito Recebido Direto</b></td>
		    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Crédito Recebido Indireto</b></td>
		    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
		</tr>
		<? 
		if($ptresacao[0]) {
			$valortotal = 0;
			$cor = 0;
			foreach($ptresacao as $acpl) { 
		?>
        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptrid_<? echo $acpl['ptrid']; ?>">
			<td align="center"><? echo $acpl['ptres']; ?></td>
			<td align="left"><? echo $acpl['descricao']; ?><input type='hidden' name='ptrid[<? echo $acpl['ptrid']; ?>]' value='<? echo $acpl['ptrid']; ?>'></td>
		    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
		    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
		    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
		    <td align="right"><? echo number_format($acpl['rofautorizado'],2,',','.'); ?></td>
		    <td align="right"><? echo number_format($acpl['rof_credrecebidodireta'],2,',','.'); ?></td>
		    <td align="right"><? echo number_format($acpl['rof_credrecebidoindireta'],2,',','.'); ?></td>
		    <td align="center"><input type="text" name="valor[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['detalhamento'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<?=$acpl['ptrid']?>','<?=number_format($acpl['detalhamento'],2,',','.')?>');" style="text-align : right; width:25ex;" title='' /></td>
		</tr>
		<? 
				$cor++;
				$valortotal = $valortotal + $acpl['detalhamento']; 
			}
		} 
		?>
        <tr style="height: 30px;">
			<td align="right" valign="top" colspan="8"><b>TOTAL :</b></td>
			<td align="center" valign="top"><input type="text" name="valortotal" id="valortotal" size="28" maxlength="" value="<? echo number_format($valortotal,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
		</tr>
        <tr>
			<td align="right" colspan="9"><input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES"></td>
		</tr>
        </table>
    </td>
</tr>



<tr>
    <td align='right' class="SubTituloDireita" valign=top>Descrição / Finalidade:</td>
    <td><?= campo_textarea( 'sbadsc', 'S', 'S', '', 80, 5, 250 );?></td>
</tr>

<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter('<?=$plicod?>');">
	  	  <? if($_REQUEST['sbaid']) { ?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar2" value="Novo" onclick="validabotao('N');">
	  	  <?}?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar3" value="Voltar" onclick="validabotao('V');"></td>
	  	  
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter(pliid) {
	if(validar()) {
		selectAllOptions( document.formulario.eqdid );
		selectAllOptions( document.formulario.entid );
		selectAllOptions( document.formulario.niveletapa );
		selectAllOptions( document.formulario.categoria );
		selectAllOptions( document.formulario.modalidade );
		document.formulario.submit();
	}
}

function marcaPermiteObras() {
	var chPermiteObras = document.getElementById("checkPermiteObras");
	var hSbaObra = document.getElementById("sbaobra");
	
	if(chPermiteObras.checked)
		hSbaObra.value = "true";
	else
		hSbaObra.value = "false";
}

function removersba(sbaid){
	var conf = confirm("Você realmente deseja excluir este Programa?");	
	if(conf) {
		document.getElementById('evento').value = 'E';
		document.getElementById('sbaid').value = sbaid;
		document.formulario.submit();	
	}
}

function alterarsba(sbaid){
	document.getElementById('evento').value = 'A';
	document.getElementById('sbaid').value = sbaid;
	document.formulario.submit();	
}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function validar(){	
	var msg = "";

	var tabela = document.getElementById('orcamento');
	// validando se existe ação selecionado/ valor
	if(tabela.rows.length == 4) {
		//msg+="A escolha das ações é obrigatório.\n";
	} else {
		for(i=2;i<(tabela.rows.length-2);i++) {
			if(!tabela.rows[i].cells[8].firstChild.value) {
				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
			}
		}
	}

	if(document.formulario.sbatitulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}
	
	
	
	<? if(!$_REQUEST['sbaid']) { ?>
		if(document.getElementById("000000003_0").value == ''){
			msg+="O preenchimento do campo Executor Orçamentário é obrigatório.\n";
		}
		if(document.getElementById("000000003_1").value == ''){
			msg+="O preenchimento do campo Gestor da SubAção é obrigatório.\n";
		}
	<? } ?>
	if(msg != ""){
		alert(msg);
		return false;
	}else return true;	
}


function abrir_lista() {
	var janela = window.open( '?modulo=principal/planotrabalho/listarProgramaSubacao&acao=A', 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
	   	alert(novo);
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }
}

function calculovalorPI() {
	var tabela = document.getElementById('orcamento');
	var tot = 0;
	for(i=2;i<tabela.rows.length-2;i++) {
		if(tabela.rows[i].cells[8].firstChild.value != "") {
			tot = tot + parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[8].firstChild.value,".",""),",","."));
		}
	}
	var c = tot.toString();
	if(c.indexOf('.') == -1) {
		document.getElementById('valortotal').value = tot.toFixed(2);
	} else {
		document.getElementById('valortotal').value = Arredonda(tot,2);
	}
	document.getElementById('valortotal').onkeyup();
}

function verificaDisponivel(campo, ptrid, vlold) {
	var linha = document.getElementById('ptrid_'+ptrid);
	valordisp = parseFloat(replaceAll(replaceAll(linha.cells[4].innerHTML,".",""),",","."));
	valoratual = parseFloat(replaceAll(replaceAll(campo.value,".",""),",","."));
	if(valoratual>valordisp) {
		alert('Valor não pode ser maior do que o Disponível');
		campo.value = vlold;
		calculovalorPI();
	}
}
function validabotao(bot) {
	form = document.formulario;
	if(bot=='N'){
		window.location='?modulo=principal/planotrabalho/programainstitucional&acao=A';
	}
	if(bot=='V'){
		window.location='?modulo=principal/planotrabalho/pesqSubacao&acao=A';
	}
}
function limitaTitulo() {
	tit = document.formulario.sbatitulo.value;
	tot_tit = tit.length;
	if(tot_tit>24){
		alert('O campo Título possui o limite de 25 caracteres.');
		document.formulario.btg.focus();
	}
}
</script>