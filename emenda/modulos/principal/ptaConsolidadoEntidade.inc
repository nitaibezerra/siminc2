<?php

$ptrid = $_REQUEST["ptrid"];

?>

<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" >
			<tr>
				<td>
					<img src="/imagens/logo_fnde.gif"/>
				</td>
				<td>
					<b>Ministério da Educação</b> <br/>
					FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO <br/>
					Simec - Sistema Integrado de Monitoramento do Ministério da Educação
				</td>
				<td align="right">
					<b>Data:</b> <?php echo Date("d/m/Y"); ?><br/>
					<b>Hora:</b> <?php echo Date("H:i:s"); ?>
				</td>
			</tr>
		</table>
		
		<br/>
		
		<?php 
		
			/* DADOS DO PLANO DE TRABALHO */
		
			$sql = "SELECT DISTINCT
						ent.enbcnpj AS cnpj,
						ent.enbnome AS orgao_entidade,
					    ent.enblog,
					    ent.enbnum,
					    ent.enbbai,
					    ent.estuf,
					    mun.mundescricao,
					    ent.enbcep,
					    ent.enbnumdddcomercial,
					    ent.enbnumcomercial,
					    ent.enbnumfax,
					    ent.enbemail,
					    ent.enborgcod,
					    bac.bconome,
					    bac.bcocod,
					    ptr.ptragenciabancaria,
					    cc.cocnu_conta_corrente
					FROM
					  	emenda.entidadebeneficiada ent
					  	inner join emenda.planotrabalho ptr
							on ent.enbid = ptr.enbid 
					    inner join territorios.municipio mun on mun.muncod = ent.muncod
					  	inner join emenda.responsavel res
					  		on res.resid=ptr.resid
					  	inner join emenda.ptemendadetalheentidade ped
					        on ped.ptrid = ptr.ptrid
					    left join emenda.banco bac
					    	on bac.bcoid = ptr.bcoid
					    left join emenda.contacorrente cc
					    	on cc.ptrid = ptr.ptrid
					WHERE
					  ptr.ptrid = {$ptrid}
					  and ptr.ptrexercicio = ".$_SESSION["exercicio"]."
					  ";
			 
			$dadosPlanoTrabalho = $db->pegaLinha($sql);
			
			$dadosPlanoTrabalho['cnpj'] = substr($dadosPlanoTrabalho['cnpj'],0,2) . "." .
										  substr($dadosPlanoTrabalho['cnpj'],2,3) . "." .
										  substr($dadosPlanoTrabalho['cnpj'],5,3) . "/" .
										  substr($dadosPlanoTrabalho['cnpj'],8,4) . "-" .
										  substr($dadosPlanoTrabalho['cnpj'],12,2);
										  
			$dadosPlanoTrabalho['enbcep'] = substr($dadosPlanoTrabalho['enbcep'],0,2) . "" .
										    substr($dadosPlanoTrabalho['enbcep'],2,3) . "-" .
										  	substr($dadosPlanoTrabalho['enbcep'],5,3);
									  
		?>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="7" style="text-align: left;">BLOCO 1 - IDENTIFICAÇÃO</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" width="20%">
					<b>CNPJ</b><br/>
					<?php echo $dadosPlanoTrabalho['cnpj']; ?>
				</td>
				<td style="border: 1px solid #ccc;" width="80%" colspan="6">
					<b>Nome</b><br/>
					<?php echo $dadosPlanoTrabalho['orgao_entidade']; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="7">
					<b>03 - Endereço (Rua, Avenida ou Praça e Nº)</b><br/>
					<?php echo $dadosPlanoTrabalho['enblog'].' '. $dadosPlanoTrabalho['enbnum']; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="4">
					<b>Complemento do Endereço (Andar, Sala etc.)</b><br/>
					&nbsp;
				</td>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>Bairro / Distrito</b><br/>
					<?php echo $dadosPlanoTrabalho['enbbai']; ?>
				</td>
			</tr>			
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>UF</b><br/>
					<?php echo $dadosPlanoTrabalho['estuf']; ?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="5">
					<b>Municipal</b><br/>
					<?php echo $dadosPlanoTrabalho['mundescricao']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>CEP</b><br/>
					<?php echo $dadosPlanoTrabalho['enbcep']; ?>
				</td>
			</tr>			
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>Caixa Postal</b><br/>
					<?php ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>DDD</b><br/>
					<?php echo $dadosPlanoTrabalho['enbnumdddcomercial']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Telefone</b><br/>
					<?php echo $dadosPlanoTrabalho['enbnumcomercial']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Fax</b><br/>
					<?php echo $dadosPlanoTrabalho['enbnumfax']; ?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>E-Mail</b><br/>
					<?php echo $dadosPlanoTrabalho['enbemail']; ?>
				</td>
			</tr>
			<tr>
				<td width="10%"></td>
				<td width="10%"></td>
				<td width="10%"></td>
				<td width="10%"></td>
				<td width="10%"></td>
				<td width="10%"></td>
				<td width="50%"></td>
			</tr>			
		</table>
		
		<br/>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="7" style="text-align: left;">BLOCO 2 - TIPO</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="2">
					<b> ESCOLA(S) MANTIDA(S) PELA ENTIDADE PRIVADA SEM FINS LUCRATIVOS - Informação exclusiva e obrigatória a ser prestada pela  Entidade Mantenedora</b><br/>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" width="20%">
					<b>Código Escola no Censo</b><br/>
				</td>
				<td style="border: 1px solid #ccc;" width="80%">
					<b>Nome</b><br/>
				</td>
			</tr>
			<?php 
			$sql = "SELECT
						ent.entcodent,
					    ent.entnumcpfcnpj,
					    ent.entnome
					FROM
						emenda.ptescolasbeneficiadas peb
					    inner join entidade.entidade ent
					    	on ent.entid = peb.entid
					WHERE
						peb.ptrid = $ptrid
						and ent.entstatus = 'A'";
			
			$arCenso = $db->carregar($sql);
			$arCenso = ($arCenso ? $arCenso : array() );
			
			foreach ($arCenso as $valor) {
				echo '<tr>
						<td style="border: 1px solid #ccc;">
							'.$valor['entcodent'].'
						</td>
						<td style="border: 1px solid #ccc;">
							'.$valor['entnome'].'
						</td>
					</tr>';
			}
			?>
		</table>
		<br/>
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="7" style="text-align: left;">BLOCO 3 - IDENTIFICAÇÃO BANCÁRIA</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" width="20%">
					<b>Código do Banco</b><br/>
					<?php echo $dadosPlanoTrabalho['bcocod']; ?>
				</td>
				<td style="border: 1px solid #ccc;" width="60%">
					<b>Nome do Banco</b><br/>
					<?php echo $dadosPlanoTrabalho['bconome']; ?>
				</td>
				<td style="border: 1px solid #ccc;" width="20%">
					<b>Código Agência/DV</b><br/>
					<?php echo $dadosPlanoTrabalho['ptragenciabancaria'] ?>
				</td>
			</tr>
		</table>
		<br/>
		
		<?php 
		//Dados do Dirigente
		
		$sql = "SELECT DISTINCT
					ent.entnumcpfcnpj,
					ent.entnome,
				    COALESCE(ende.endlog,'&nbsp;') as endlog,
				    COALESCE(ende.endnum,'&nbsp;') as endnum,
				    COALESCE(ende.endbai,'&nbsp;') as endbai,
				    COALESCE(ende.estuf,'&nbsp;') as estuf,
				    COALESCE(mun.mundescricao,'&nbsp;') as mundescricao,
				    COALESCE(ende.endcep,'&nbsp;') as endcep,
				    COALESCE(ent.entnumdddcomercial,'&nbsp;') as entnumdddcomercial,
				    COALESCE(ent.entnumcomercial,'&nbsp;') as entnumcomercial,
				    COALESCE(ent.entnumfax,'&nbsp;') as entnumfax,
				    COALESCE(ent.entemail,'&nbsp;') as entemail,
				    case when ent.entsexo = 'M' then 'Masculino'
				    	 when ent.entsexo = 'F' then 'Feminino'
				    	 else '&nbsp;' end as sexo,
				    COALESCE(fun.fundsc,'&nbsp;') as fundsc,
				    COALESCE(ent.entnumrg,'&nbsp;') as entnumrg,
				    COALESCE(ent.entorgaoexpedidor,'&nbsp;') as entorgaoexpedidor 
				FROM
				  	entidade.entidade ent
				  	inner join emenda.planotrabalho ptr on ent.entid = ptr.entiddirigente 
				    left join entidade.endereco ende  
				    	inner join territorios.municipio mun on mun.muncod = ende.muncod
					    inner join territorios.estado est on est.estuf = mun.estuf
			  		on ende.entid = ent.entid 
			  			and ende.endstatus = 'A'
				  	left join entidade.funcaoentidade fue
				    	on fue.entid = ent.entid
				        and fue.fuestatus = 'A'
				    left join entidade.funcao fun
				    	on fun.funid = fue.funid
				        and fun.funstatus = 'A'
				WHERE
				  ptr.ptrid = $ptrid
				  and ptr.ptrexercicio = {$_SESSION['exercicio']}
				  ";

		$arDirigente = $db->pegaLinha($sql);
		?>
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="7" style="text-align: left;">BLOCO 4 - IDENTIFICAÇÃO  DO DIRIGENTE</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>CPF</b><br/>
					<?php echo $arDirigente['entnumcpfcnpj'];?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="4">
					<b>Nome</b><br/>
					<?php echo $arDirigente['entnome'];?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Sexo</b><br/>
					<?php echo $arDirigente['sexo'];?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="6">
					<b>Endereço (Rua, Avenida ou Praça e Nº)</b><br/>
					<?php echo $arDirigente['endlog'].' '.$arDirigente['endnum'];?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="4">
					<b>Complemento do Endereço (Andar, Sala etc.)</b><br/>
					<?php echo '&nbsp;';?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="2">
					<b>Bairro / Distrito</b><br/>
					<?php echo $arDirigente['endbai'];?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>UF</b><br/>
					<?php echo $arDirigente['estuf'];?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="4">
					<b>Município</b><br/>
					<?php echo $arDirigente['mundescricao'];?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="2">
					<b>CEP</b><br/>
					<?php echo $arDirigente['endcep'];?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>DDD</b><br/>
					<?php echo $arDirigente['entnumdddcomercial'];?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Telefone</b><br/>
					<?php echo $arDirigente['entnumcomercial'];?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Fax</b><br/>
					<?php echo $arDirigente['entnumfax'];?>
				</td>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>e-mail</b><br/>
					<?php echo $arDirigente['entemail'];?>
				</td>
			</tr>
				<td style="border: 1px solid #ccc;" colspan="4">
					<b>Cargo ou Função</b><br/>
					<?php echo $arDirigente['fundsc'];?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Nº Carteira de Identidade</b><br/>
					<?php echo $arDirigente['entnumrg'];?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Órgão Expedidor/UF</b><br/>
					<?php echo $arDirigente['entorgaoexpedidor'];?>
				</td>
			</tr>
			<tr>
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<td></td>
			</tr>
		</table>
		<br/>
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="7" style="text-align: left;">BLOCO 5 - AUTENTICAÇÃO</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>Local e Data</b><br/>
					<?php echo '&nbsp;';?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>Nome do Dirigente ou Representante Legal</b><br/>
					<?php echo $arDirigente['entnome'];?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>Assinatura  Dirigente ou Representante Legal</b><br/>
					<?php echo '&nbsp;';?>
				</td>
			</tr>
		</table>		
	</body>
	<table width="95%" align="center">
		<tr>
			<td>
				<input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
		
	</body>
</html>