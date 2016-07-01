<?php

header( 'Content-type: text/html; charset=iso-8859-1' );

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "monitoraseb/classes/Subacao.class.inc";
include_once APPRAIZ . "www/monitoraseb/_constantes.php";

$sa = new SubAcao();
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraComboSubAcao'){

    $coordenacaoId = $_REQUEST['coordenacaoId'];
    $sbaid = $_REQUEST['sbaid'];
    $funcaoCombo = $_REQUEST['funcaoCombo'];
    $permissao = $_REQUEST['permissao'];
    $cootec = $_REQUEST['cootec'];

    $where = "WHERE ";

    if($cootec && !empty($cootec)){
        $where .= "sac.usucpf = '$cootec' AND ";
    }

    $sql = "SELECT sac.sbaid as codigo
                  ,case when sa.sbasigla is null or char_length(sa.sbasigla)=0 then sa.sbatitulo else sa.sbasigla || ' - ' || sa.sbatitulo end as descricao
              FROM monitoraseb.subacaocoordenacao sac
             INNER JOIN monitora.pi_subacao sa
                ON sa.sbaid = sac.sbaid
             INNER JOIN workflow.documento d
                ON sac.docid = d.docid
            $where sac.coonid = $coordenacaoId
               AND d.esdid = ".WF_ESTADO_APROVACAO."
             order by sa.sbatitulo";

    die($db->monta_combo('sbaid', $sql, $permissao, 'Selecione...', $funcaoCombo, '', '', '', $funcaoCombo == 'mostraListaAcao' ? 'S' : 'N','sbaid', '', $sbaid, 'Subação'));

}

elseif (isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraComboCoordenacao'){

    $cootec = $_REQUEST['cootec'];
    $sisid = $_SESSION['sisid'];
    $coonid = $_REQUEST['coonid'];
    $permissao = $_REQUEST['permissao'];

    $sql = "select distinct c.coonid as codigo, c.coosigla || ' - ' || c.coodsc as descricao
              from monitoraseb.usuarioresponsabilidade ur
             inner join seguranca.perfil p
                on p.pflcod = ur.pflcod
             inner join monitoraseb.coordenacao c
                on ur.coonid = c.coonid
             where p.pflnivel = 3
               and ur.rpustatus = 'A'
               and ur.usucpf = '$cootec'";

    $resultado = $db->carregar($sql);

    die($db->monta_combo('coonid', $resultado, $permissao, count($resultado) > 1 ? 'Selecione...' : '', 'listarSubacao', '', '', '200', 'S', 'coonid', '', $coonid, 'Coordenação'));
}

elseif(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraComboCursoMestre'){

    $coonid = $_REQUEST['coonid'];
    $sbaid = $_REQUEST['sbaid'];
    $cmtid = $_REQUEST['cmtid'];
    $permissao = $_REQUEST['permissao'];

    $sql = "select cm.cmtid as codigo
                  ,cm.cmtdsc as descricao
              from monitoraseb.cursomestre cm
             inner join monitoraseb.subacaocoordenacao sc
                on cm.scoid = sc.scoid
             inner join workflow.documento d
                ON cm.docid = d.docid
             where sc.coonid = $coonid
               and sc.sbaid = $sbaid
               and d.esdid = ".WF_ESTADO_APROVACAO."
             order by cmtdsc";

    die($db->monta_combo('cmtid', $sql, $permissao, 'Selecione...', '', '', '', '200', 'S','cmtid', '', $cmtid));
}

elseif(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraListaAcao'){
    $subacaoId = $_REQUEST['subacaoId'];
    $sql = "SELECT distinct aca.acacod, aca.acadsc FROM monitora.acao aca
                 inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
                 inner JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
                 inner JOIN public.unidade uni ON uni.unicod = dtl.unicod
                WHERE  sd.sbaid = ".$subacaoId." order by aca.acadsc";
    $cabecalho = array("Código","Ação");
    die($db->monta_lista_simples($sql,$cabecalho,100,50,'N','95%','N',false,false,false,true));
}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraListaGradeCurricular'){
	$i=0;

	$listagem = $_SESSION['listagemGradeCurricular'];
	$permissao = $_REQUEST['permissao'];
	unset ($_SESSION['listagemGradeCurricular']);
	$listagemExibida = null;

	if($_REQUEST['itemLista']!='null' && $_REQUEST['itemLista']!=""){
		unset($listagem[$_REQUEST['itemLista']]);
	}

	if($listagem && !empty($listagem)){
		foreach ($listagem as $itemLista){
			$acaoLista = ($permissao!='N') ? "<img onclick='alterarGradeCr($i)' class='link' title='Alterar' alt='Alterar' src='../imagens/alterar.gif'>
							<img onclick=\"if(confirm('Deseja excluir o registro?')){excluirGradeCr($i)}\" class='link' title='Excluir' alt='Excluir' src='../imagens/excluir.gif'>":"";
			$novoItem = array('acao'=> $acaoLista, 'tgcdsc'=> $itemLista['tgcdsc'], 'gcrnome'=> $itemLista['gcrnome'], 'gcrnumhoraula'=> $itemLista['gcrnumhoraula'], 'gcrementa'=> $itemLista['gcrementa'] );
			$listagemExibida[$i] =  $novoItem;
			$_SESSION['listagemGradeCurricular'][$i] = $itemLista;
			$i++;
		}
	}

	$cabecalho = array("Ação","Tipo","Nome","Carga Horária","Ementa");
	die($db->monta_lista_simples(($listagemExibida) ? $listagemExibida : array(),$cabecalho,100,50,'N','95%','N',false,false,false,true));
}

elseif(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'associacaoPerfil'){

		$listaIdPerfis = $_REQUEST['listaIdPerfis'];
		$paramSisID = $_REQUEST['sisID'];
		$paramUsuCFP = $_REQUEST['usuCFP'];

		$sql = "SELECT * FROM monitoraseb.tiporesponsabilidade
				WHERE tprsnvisivelperfil = 't' ORDER BY tprdsc";

		$responsabilidades = (array) $db->carregar($sql);
		if(!empty($listaIdPerfis)){
			$sqlPerfisUsuario = "SELECT p.pflcod, p.pfldsc
								 FROM seguranca.perfil p
								 WHERE p.pflstatus='A' AND p.pflcod in(".$listaIdPerfis.")  ORDER BY p.pfldsc";
			$query = sprintf($sqlPerfisUsuario, $paramUsuCFP);
			$perfisUsuario = $db->carregar($query);
		}
	?>
	<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
		<tr>
			<td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
			<td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
			<td align="center" colspan="<?=@count($responsabilidades)?>" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
		</tr>
		<tr>
			<?php
			foreach( $responsabilidades as $responsabilidade ):
			?>
				<td align="center" bgcolor="#e9e9e9" align="center"><?= $responsabilidade["tprdsc"] ?></td>
			<?
				$javascript = "";
			endforeach;
			?>
		</tr>
		<?php if(!empty($perfisUsuario)){ foreach( $perfisUsuario as $perfil ): ?>
			<?php
				$marcado = $i++ % 2 ? '#F7F7F7' : '';
				$sqlResponsabilidadesPerfil = "SELECT p.*, tr.tprdsc, tr.tprsigla
											   FROM (SELECT * FROM monitoraseb.tprperfil
											   WHERE pflcod = '%s') p
											   RIGHT JOIN monitoraseb.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
											   WHERE tprsnvisivelperfil = TRUE
											   ORDER BY tr.tprdsc";
				$query = sprintf($sqlResponsabilidadesPerfil, $perfil["pflcod"]);

				$responsabilidadesPerfil = (array) $db->carregar($query);

				// Esconde a imagem + para perfis sem responsabilidades
				$mostraMais = false;

				foreach ( $responsabilidadesPerfil as $resPerfil ) {
					if ( (boolean) $resPerfil["tprcod"] ){
						$mostraMais = true;
						break;
					}
				}
			?>
			<tr bgcolor="<?=$marcado?>">
				<td style="color: #003c7b">
					<? if ($mostraMais): ?>
						<a href="Javascript:abreconteudo('../monitoraseb/geral/cadastro_responsabilidades.php?usucpf=<?=$paramUsuCFP?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
							<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$perfil["pflcod"]?>"/>
						</a>
					<?php endif; ?>
				</td>
				<td><?=$perfil["pfldsc"]?></td>
				<?php foreach( $responsabilidadesPerfil as $resPerfil ): ?>
					<td align="center">
						<?php if ( (boolean) $resPerfil["tprcod"] ): ?>
							<input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir" onclick="popresp_<?=$paramSisID?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')">
						<?php else: ?>
							-
						<?php endif; ?>
					</td>
				<?php endforeach; ?>
			</tr>
			<tr bgcolor="<?=$marcado?>">
				<td colspan="10" id="td<?=$perfil["pflcod"]?>"></td>
			</tr>
		<?php endforeach; }?>
	</table>
	<?php
}

elseif (isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraComboCoordenacaoSemSubacao'){

    $cootec = $_REQUEST['cootec'];
    $coonid = $_REQUEST['coonid'];

    die($sa->comboCoordenacao($coonid, $cootec));
}

elseif (isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraComboPublicoAlvoCM'){

    $cmtid = $_REQUEST['cmtid'];
    $sbaid = $_REQUEST['sbaid'];
    $permissao = $_REQUEST['permissao'];

    global $funid;
	if($cmtid && !empty($cmtid)){
        $sql = "select distinct f.funid as codigo
                      ,f.fundsc as descricao
                  from entidade.funcao f
                 inner join monitoraseb.cursomestrepublicoalvo pa
                    on f.funid = pa.funid
                 inner join monitoraseb.cursomestre cm
                    on pa.cmtid = cm.cmtid
                 inner join monitoraseb.subacaocoordenacao sc
                    on cm.scoid = sc.scoid
                 inner join monitoraseb.subacaopublicoalvo sbp
                    on sbp.funid = f.funid
                   and sbp.sbaid = sc.sbaid
                 where f.funstatus = 'A'
                   and f.funtipo = 'F'
                   and cm.cmtid = $cmtid
                   and sc.sbaid = $sbaid
                 order by descricao";
        $funid = $db->carregar($sql);
	}else{
		$funid = array();
	}
        $sql = "select  distinct f.funid as codigo,f.fundsc as descricao
                	from entidade.funcao f
                  	join monitoraseb.subacaopublicoalvo sbp
						on sbp.funid = f.funid
                 	where f.funstatus = 'A'
                   		and f.funtipo = 'F'
                   		and sbp.funid = f.funid
                   		and sbp.sbaid = $sbaid
                 	order by descricao";

    combo_popup('funid', $sql, 'Selecione o(s) Público(s)-alvo', '360x460',0,array(),'',$permissao );
	echo'<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraListaEquipe'){
	$i=0;

	$permissao = $_REQUEST['permissao'];
	$listagem = $_SESSION['listagemEquipe'];
	unset ($_SESSION['listagemEquipe']);
	$listagemExibida = null;

	if($_REQUEST['itemLista']!='null' && $_REQUEST['itemLista']!=""){
		unset($listagem[$_REQUEST['itemLista']]);
	}

	if($listagem && !empty($listagem)){
		foreach ($listagem as $itemLista){
			$acaoLista = ($permissao!="N")?"	<img onclick='alterarEquipe(".$i.")' class='link' title='Alterar' alt='Alterar' src='../imagens/alterar.gif'>
							<img onclick=\"if(confirm('Deseja excluir o registro?')){excluirEquipe(".$i.")}\" class='link' title='Excluir' alt='Excluir' src='../imagens/excluir.gif'>":"";
			$novoItem = array('acao'=> $acaoLista, 'catnome'=> $itemLista['catnome'], 'ecmdscfuncao'=> $itemLista['ecmdscfuncao'], 'ecmdscatribuicao'=> $itemLista['ecmdscatribuicao'], 'unrdsc'=> $itemLista['unrdsc'], 'ecmnummin'=> $itemLista['ecmnummin'], 'ecmnummax'=> $itemLista['ecmnummax'] );
			$listagemExibida[$i] =  $novoItem;
			$_SESSION['listagemEquipe'][$i] = $itemLista;
			$i++;
		}
	}

	$cabecalho = array("Ação","Categoria","Função","Atribuição","Unidade Referência","Qtd. Mínima","Qtd. Máxima");
	die($db->monta_lista_simples(($listagemExibida) ? $listagemExibida : array(),$cabecalho,100,50,'N','95%','N',false,false,false,true));
}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraListaOfertaVaga'){
	$i=0;

	$permissao = $_REQUEST['permissao'];
	$listagem = $_SESSION['listagemOfertaVaga'];
	unset ($_SESSION['listagemOfertaVaga']);
	$listagemExibida = null;

	if($_REQUEST['itemLista']!='null' && $_REQUEST['itemLista']!=""){
		unset($listagem[$_REQUEST['itemLista']]);
	}

	if($listagem && !empty($listagem)){
		foreach ($listagem as $itemLista){
			$acaoLista = ($permissao!="N")?"	<img onclick='alterarVaga(".$i.")' class='link' title='Alterar' alt='Alterar' src='../imagens/alterar.gif'>
							<img onclick=\"if(confirm('Deseja excluir o registro?')){excluirVaga(".$i.")}\" class='link' title='Excluir' alt='Excluir' src='../imagens/excluir.gif'>":"";
			$novoItem = array('acao'=> $acaoLista, 'ovgnumvagas'=> $itemLista['ovgnumvagas'], 'ovganobase'=> $itemLista['ovganobase']);
			$listagemExibida[$i] =  $novoItem;
			$_SESSION['listagemOfertaVaga'][$i] = $itemLista;
			$i++;
		}
	}

	$cabecalho = array("Ação","Vagas","Ano");
	die($db->monta_lista_simples(($listagemExibida) ? $listagemExibida : array(),$cabecalho,100,50,'N','95%','N',false,false,false,true));
}
elseif (isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'atualizaDocumentoAnexo'){

    $donoId = $_REQUEST['donoId'];
    $donoClass = $_REQUEST['donoClass'];
    $arqid = $_REQUEST['arqid'];
    $permissaoGravar = $_REQUEST['permissao'];

    if(!empty($donoClass) && $donoClass == "CursoMestre" && !empty($donoId)){
    	include_once APPRAIZ . "monitoraseb/classes/CursoMestre.class.inc";
    	$dClass = new CursoMestre();
    	if($arqid && !empty($arqid)){
    		$dClass->excluirAnexo($arqid);
    	}
    	die($dClass->listaDocumentos($donoId,$permissaoGravar));
    }
    if(!empty($donoClass) && $donoClass == "Subacao" && !empty($donoId)){
    	include_once APPRAIZ . "monitoraseb/classes/Subacao.class.inc";
    	$dClass = new Subacao();
    	if($arqid && !empty($arqid)){
    		$dClass->excluirAnexo($arqid);
    	}
    	die($dClass->listaDocumentos($donoId,$permissaoGravar));
    }

}
elseif (isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'mostraCadastro'){
	include APPRAIZ."monitoraseb/classes/Categoria.class.inc";
	$categoria = new Categoria();
	$categoria->carregarPorId($_REQUEST['idCategoria']);
	$arrDados = $categoria->getDados();
	extract($arrDados);
	$catvalfcheck = $catvalidacao=='f'?"CHECKED":"";
	$catvaltcheck = ($catvalidacao!=null)?($catvalidacao=='t'?"CHECKED":""):"CHECKED";
	$catbolfcheck = $catbolsista=='f'?"CHECKED":"";
	$catboltcheck = ($catbolsista!=null)?($catbolsista=='t'?"CHECKED":""):"CHECKED";
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Nome:</td>
			<td>';
	echo campo_texto('catnome','S','S','Nome da Categoria',100,100,'','');
	echo 	'</td>
		</tr>
		<tr>
					<td width="25%" class="SubtituloDireita">Sujeito a Validação:</td>
					<td >
			            <input type="radio" name="catvalidacao" value="t" '.$catvaltcheck.'> Sim
						<input type="radio" name="catvalidacao" value="f" '.$catvalfcheck.'> Não
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Bolsista:</td>
					<td >
			            <input type="radio" name="catbolsista" value="t" '.$catboltcheck.' onchange="habilitaDesabilitaValorBolsa(\'S\');"> Sim
						<input type="radio" name="catbolsista" value="f" '.$catbolfcheck.' onchange="habilitaDesabilitaValorBolsa(\'N\');"> Não
					</td>
				</tr>
		<tr>
			<td width="25%" class="SubtituloDireita">Número do Documento Legal:</td>
			<td >';
	echo campo_texto("catnumdoclegal","S",($catbolsista!='f')?'S':'N',"Número do Documento Legal",20,20,"","");
	$catvalunibolsa = ($catvalunibolsa!=null)? $categoria->formataNumeric2Moeda($catvalunibolsa) : null;
	echo'</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita">Valor Unitário da Bolsa:</td>
			<td >'.campo_texto('catvalunibolsa','S',($catbolsista!='f')?'S':'N','Valor Unitário da Bolsa',12,11,'[###.]###,##','').'</td>
		</tr>
		<tr>
			<th></th>
			<th style="text-align: left;">
				<input type="hidden" name="catid" value="'.$catid.'"/>
				<input type="hidden" name="requisicao" value="salvar" />
				<input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" onclick="gravar();">
			</th>
		</tr>
	</table>';
}
?>