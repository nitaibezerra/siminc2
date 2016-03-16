<?php
/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ . 'includes/workflow.php';
print '<br/>';
montar_titulo_projeto();





if ($_REQUEST['gerarArvore'])
{

	extract ($_REQUEST);
	
	$exportados =  array();
	$ct = 0;

	function insereitenschecklist($atiid,$novoatiid,$atinumero,$atidescricao){
	
		global $db;
		$sql = "select iclid , icldsc, iclcritico, iclordem from  pde.itemchecklist where atiid = {$atiid} order by iclordem";
		$dadosicl = $db->carregar($sql);
			
		if ($dadosicl)
		{
			foreach ($dadosicl as $di)
			{
				$di['icldsc'] = str_replace("'", "´", $di['icldsc']);
				
				$sql = "insert into pde.itemchecklist (atiid, icldsc, iclcritico, iclordem) values ('{$novoatiid}', '{$di['icldsc']}', '{$di['iclcritico']}', {$di['iclordem']}) returning iclid";
				$novoiclid = $db->pegaUm($sql);
					
				$tpdid   = TPDID_ENEM;
				$docdsc = $novoiclid." - ".$di['icldsc']."<p>".$atinumero." - ".$atidescricao."</p>";
				$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
				 
				$db->executar("UPDATE pde.itemchecklist SET docid='".$docid."' WHERE iclid='".$novoiclid."'");
				
				$sql = "select entid, tpvid from pde.checklistentidade where iclid = {$di['iclid']}";
				$dadosclientidade = $db->carregar($sql);
	
				if ($dadosclientidade)
				{
					foreach ($dadosclientidade as $dci)
					{
						$sql = "insert into pde.checklistentidade (iclid, entid,tpvid) values ({$novoiclid},{$dci['entid']}, {$dci['tpvid']} )";
	
						$db->executar($sql);
					}
				}
	
				$sql = "select tpvid,etcopcaoevidencia, etcevidencia from pde.etapascontrole where iclid= {$di['iclid']} ";
				$dadosetc = $db->carregar($sql);
					
				if ($dadosetc)
				{
					foreach ($dadosetc as $det)
					{
						$det['etcopcaoevidencia'] = str_replace("'", "´", $det['etcopcaoevidencia']);
						$det['etcevidencia']	  = str_replace("'", "´", $det['etcevidencia']);
						
						$sql = "insert into pde.etapascontrole (iclid, tpvid, etcopcaoevidencia,etcevidencia) values ({$novoiclid},{$det['tpvid']}, '{$det['etcopcaoevidencia']}', '{$det['etcevidencia']}' )";
	
						$db->executar($sql);
					}
				}
	
			}
	
		}
	}
	
	
	//funcão de chamada recursiva para exportação dos nós de arvores de atividades
	function incluinoarvore($atiidno,$atiidpai)
	{
		


		global $ct;
		global $db;
		global $sqls;
		global $exportados;
		global $ordem;
		global $elementoRaiz;
		global $checklist;
		
		$ct++;
		
		$flag = true;
		while ($flag)
		{
			
			
			//select que busca os dados do nó, restringindo os que jão foram exportados
			$sql = "select atiid, atidescricao, atiordem, tatcod, esaid, atitipoenem,
			_atiprojeto, _atinumero, _atiprofundidade, _atiordem, _atifilhos, _atiirmaos
			from pde.atividade 
			where atistatus = 'A' and atiid = {$atiidno} ".($exportados?" and atiid not in (".implode(',', $exportados).")":"");
			
			$d = $db->pegaLinha($sql);
			//inclui o nó caso ele nao tenha sido exportado ainda
			if ($d)
			{
			
				if ($ct == 1)
				{
					$d['atidescricao']  = $elementoRaiz;
					$d['atiordem'] 		= $ordem;
					$d['_atiordem'] = '00'. str_pad($ordem,2,"0",STR_PAD_LEFT);
					$d['_atinumero']	= $ordem;
					
				}
				
				$d['atidescricao'] = str_replace("'", "´", $d['atidescricao']);
			
				$sql = "insert into pde.atividade (atidescricao,atiordem,atiidpai, atistatus, tatcod, esaid, atitipoenem,atitipoandamento,
				_atiprojeto, _atinumero, _atiprofundidade, _atiordem, _atifilhos, _atiirmaos, atiexportado ) values ('{$d['atidescricao']}', {$d['atiordem']}, {$atiidpai}, 'A', '{$d['tatcod']}', 1, '{$d['atitipoenem']}', '{$d['atitipoandamento']}',
				{$d['_atiprojeto']}, '{$d['_atinumero']}', '{$d['_atiprofundidade']}', '{$d['_atiordem']}', '{$d['_atifilhos']}', '{$d['_atiirmaos']}', true) returning atiid";
			
				
				$pai = $db->pegaUm($sql);
				$db->commit();
				array_push($exportados,$d['atiid']);
				
				
				if ($checklist)
					insereitenschecklist($d['atiid'], $pai,$d['_atinumero'],$d['atidescricao']);
				
				//vai incluindo todos os niveis inferiores de uma ramificação, até chegar no ultimo filho do ramo.
				incluiNoArvore($d['atiid'],$pai);
			}
			
			//select que busca os dados do filhos do nó, restringindo os que jão foram exportados
			$sql = "select atiid, atidescricao, atiordem, tatcod, esaid,atitipoenem,
			_atiprojeto, _atinumero, _atiprofundidade, _atiordem, _atifilhos, _atiirmaos from pde.atividade where atistatus = 'A' and atiidpai = {$atiidno} 
			".($exportados?"and atiid not in (".implode(',', $exportados).")":"")." order by atiordem limit 1";
		
			$d2 = $db->pegaLinha($sql);
			//se ainda houver filho, inclui
			if ($d2)
			{
			
				$posicao = strpos($d2['_atinumero'], ".");  
				
				
				$d2['_atinumero'] = $ordem.'.' . substr($d2['_atinumero'], $posicao+1);
				
				$d2['_atiordem'] = '00'. str_pad($ordem,2,"0",STR_PAD_LEFT) . substr($d2['_atiordem'], 4);
				
				$d2['atidescricao'] = str_replace("'", "´", $d2['atidescricao']);
				
				$sql = "insert into pde.atividade (atidescricao,atiordem,atiidpai, atistatus,  tatcod, esaid, atitipoenem, atitipoandamento,
				_atiprojeto, _atinumero, _atiprofundidade, _atiordem, _atifilhos, _atiirmaos, atiexportado ) values 
				('{$d2['atidescricao']}', {$d2['atiordem']}, {$atiidpai}, 'A', '{$d2['tatcod']}', 1, '{$d2['atitipoenem']}', '{$d2['atitipoandamento']}',
				{$d2['_atiprojeto']}, '{$d2['_atinumero']}', '{$d2['_atiprofundidade']}', '{$d2['_atiordem']}', '{$d2['_atifilhos']}', '{$d2['_atiirmaos']}', true) returning atiid";
		
				$pai = $db->pegaUm($sql);
				$db->commit();

				array_push($exportados,$d2['atiid']);
				
				if ($checklist)
					insereitenschecklist($d2['atiid'], $pai, $d2['_atinumero'], $d2['atidescricao']);
				
				incluiNoArvore($d2['atiid'],$pai);
			}
			//volta para os niveis superiores da arvore
			else
			{
				return;
			}
		}
		
	}
	
	
	
	if ($atiid == '' || $elementoRaiz == '')
	{
		echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dados informados inválidos';
	}
	else 
	{
		
		$sql = "select atiid from pde.atividade where atiidpai = (select atiidpai from pde.atividade where atiid = {$atiid})
		and atistatus = 'A'
		and atiordem = {$ordem}";
		
		
		$dados = $db->pegaLinha($sql);
		
		if ( $dados )
		{
			echo('<P>impossível exportar arvore, nível já existente</P>');
		}
		else
		{
		
			$sql = "select atiidpai from pde.atividade where atiid = {$atiid}";
		
			$dados = $db->pegaLinha($sql);
		
			
			
			incluinoarvore($atiid,$dados['atiidpai']);
			
			echo "<script>alert('Dados exportados com sucesso');</script>";
		}
		
	}
	
}

if ($_SESSION['usucpf'] != '' && $_SESSION['usucpf'] != '')
{
	echo 'Acesso não autorizado';
}
else
{
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<form name="formulario_filtro_arvore" action="" method="post" onsubmit="return confirm('Deseja realmente exportar a árvore ?');">
				<input type="hidden" name="gerarArvore" value="1"/>
				<table border="0" cellpadding="5" cellspacing="0" width="100%">
					<tr>
						<td align="right" width="150">
							Fluxo:
						</td>
						<td>
							<?php 
							$sql = "select atiid as codigo, atidescricao as descricao from pde.atividade where atiidpai = 114098 and atistatus = 'A' order by 2";
							$db->monta_combo('atiid', $sql, 'S', '', '','','','','atiid');
							?>
						</td>
					</tr>
					<tr>
						<td align="right" width="150">
							Nome do elemento raiz destino:
						</td>
						<td>
							<input type='text' id='elementoRaiz', name='elementoRaiz'>
						</td>
					</tr>
					<tr>
						<td align="right" width="150">
							Ordem:
						</td>
						<td>
							<input type='text' id='ordem', name='ordem'>
						</td>
					</tr>
					<tr>
						<td align="right" width="150">
							Exportar checklist
						</td>
						<td>
							<input type='checkbox' id='checklist', name='checklist'>
						</td>
					</tr>
					<tr>
					<td colspan=2>
						<input type='submit' value='Criar' >
					</td>
					</tr>
				</table>
			</form>
		</td>
	</tr>
</table>
<?php 
}
?>
