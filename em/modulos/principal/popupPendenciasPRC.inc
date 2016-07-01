<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>	    
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <body>
		<?php
		
		if(!$_SESSION['emiid']){
			echo "<script>
					alert('Erro de variável, refaça o procedimento!');
					window.close();
				  </script>";
			exit;
		}
		
		monta_titulo("Pendências", "PRC 2012");
		echo montaCabecalhoPRC($_SESSION['emiid'], null, null, false);
		//echo "<br/>";
		
		$sql = "select 
				m.mcpid,
				m.mcpdsc, 
				g.papid, 
				t.mdoid, 
				b.benid, 
				p.preid 
			from 
				em.macrocampo m
			left join 
				em.emigap g on g.mcpid = m.mcpid and g.emiid = {$_SESSION['emiid']}
			left join 
				em.emimatrizdistribuicaoorcamentargap t on t.papid = g.papid
			left join 
				em.emibeneficiario b on b.benid = g.benid
			left join 
				em.emiprofissionalenvolvido p on p.mcpid = m.mcpid and p.emiid = {$_SESSION['emiid']}
			order by 
				m.mcpid, 
				g.papid, 
				t.mdoid, 
				b.benid, 
				p.preid";
	
		$rsMacrocampo = $db->carregar($sql);
		$arMacros = array();
		//ver($sql);
		?>
		<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
			<?php foreach($rsMacrocampo as $dados): ?>
				<?php if(in_array($dados['mcpid'], array(1,2))): ?>					
					<?php if((empty($dados['papid']) || empty($dados['mdoid']) || empty($dados['benid']) || empty($dados['preid'])) && !in_array($dados['mcpid'], $arMacros)): ?>
						<tr>
							<td class="subtituloEsquerda"><?php echo 'Macrocampo: '.$dados['mcpdsc'] ?></td>
							<td>
								<?php
								$arPendencias = array();
								
								empty($dados['papid']) ? $arPendencias[] = 'Ação/Atividade e Meta' : '';
								empty($dados['mdoid']) ? $arPendencias[] = 'Matriz' : '';
								empty($dados['benid']) ? $arPendencias[] = 'Beneficiários' : '';
								empty($dados['preid']) ? $arPendencias[] = 'Profissionais de Educação' : '';
								
								echo 'Verificar preenchimento: '.implode(', ',$arPendencias);
								$arMacros[] = $dados['mcpid'];
								?>
							</td>
						</tr>
					<?php endif; ?>					
				<?php else: ?>
					<?php if(!empty($dados['papid']) || !empty($dados['mdoid']) || !empty($dados['benid']) || !empty($dados['preid'])): ?>
						<?php if((empty($dados['papid']) || empty($dados['mdoid']) || empty($dados['benid']) || empty($dados['preid'])) && !in_array($dados['mcpid'], $arMacros)): ?>
							<tr>
								<td class="subtituloEsquerda"><?php echo 'Macrocampo: '.$dados['mcpdsc'] ?></td>
								<td>
									<?php
									$arPendencias = array();
									
									empty($dados['papid']) ? $arPendencias[] = 'Ação/Atividade e Meta' : '';
									empty($dados['mdoid']) ? $arPendencias[] = 'Matriz' : '';
									empty($dados['benid']) ? $arPendencias[] = 'Beneficiários' : '';
									empty($dados['preid']) ? $arPendencias[] = 'Profissionais de Educação' : '';
									
									echo 'Verificar preenchimento: '.implode(', ',$arPendencias);
									$arMacros[] = $dados['mcpid'];
									?>
								</td>
							</tr>
						<?php endif; ?>
					<?php endif; ?>
				<?php endif; ?>			
			<?php endforeach; ?>
			
			<?php
			$sql = "SELECT ent.* FROM entidade.entidade ent 
					LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					WHERE fen.funid='".ENTIDADE_FUNID_DIRETOR."' AND fea.entid='{$_SESSION['entid']}'";
			
			$rsDiretor = $db->pegaLinha($sql);
			?>	
			<?php if(!$rsDiretor): ?>						
				<tr>
					<td class="subtituloEsquerda">Cadastro - Diretor</td>
					<td>Pendente</td>
				</tr>
			<?php endif; ?>
			
			<?php
			$sql = "SELECT ent.* FROM entidade.entidade ent 
					LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					WHERE fen.funid='".ENTIDADE_FUNID_COORDENADOR."' AND fea.entid='{$_SESSION['entid']}'";
			
			$rsArticulador = $db->pegaLinha($sql);
			?>	
			<?php if(!$rsArticulador): ?>						
				<tr>
					<td class="subtituloEsquerda">Professor Articulador</td>
					<td>Pendente</td>
				</tr>
			<?php endif; ?>
			
			<?php
			$sql = "select etjid from em.emiensinomedioinovador where emiid = ".$_SESSION['emiid'];
			$rsJornada = $db->pegaUm($sql);
			?>			
			<?php if(!$rsJornada): ?>
				<tr>
					<td class="subtituloEsquerda">Jornada Escolar</td>
					<td>Verificar preenchimento</td>
				</tr>
			<?php endif; ?>
			<?php
			$sql = "select emiid from em.emiescolaturno where emiid = ".$_SESSION['emiid'];
			$rsTurno = $db->pegaUm($sql);
			?>			
			<?php if(!$rsTurno): ?>
				<tr>
					<td class="subtituloEsquerda">Turno(s) da Escola</td>
					<td>Verificar preenchimento</td>
				</tr>
			<?php endif; ?>		
			
			<?php 
//			$sql = "select 
//						emi.emiid,
//						ede.estuf,
//						ent.entnome,	
//						tjo.etjjornada,	
//						est.estdescricao,
//						(SELECT
//							coalesce(sum(cen.emcquantidadealunos),0) as total 
//						FROM 
//							em.emicenso cen
//						WHERE 
//							cen.entid::integer = ent.entid)	as alunos,
//						coalesce((SELECT 
//							sum(rcc.emrcusteio+rcc.emrcapital) as total
//						FROM 
//							em.emireccusteiocapital rcc
//						WHERE 
//							rcc.emrjornada = tjo.etjjornada 
//						AND 
//							(SELECT
//								coalesce(sum(cen.emcquantidadealunos),0) as total 
//							FROM 
//								em.emicenso cen
//							WHERE 
//								cen.entid::integer = ent.entid) BETWEEN rcc.emrqtdinialunos 
//						AND 
//							rcc.emrqtdfinalunos),0) as recurso,
//						coalesce((select 
//							sum(mdovalorunitario*mdoqtd) 
//						from 
//							em.emimatrizdistribuicaoorcamentargap gt1
//						join 
//							em.emigap ga1 on ga1.papid = gt1.papid 
//						where 
//							ga1.emiid = emi.emiid),0) as itens
//					from 
//						em.emiensinomedioinovador emi
//					inner join 
//						entidade.entidade ent on ent.entid = emi.entid
//					inner join 
//						entidade.endereco ede on ede.entid = ent.entid
//					inner join 
//						em.emitipojornada tjo on tjo.etjid = emi.etjid
//					inner join
//						territorios.estado est on upper(est.estuf) = upper(ede.estuf)
//					where 
//						emi.entid = {$_SESSION['entid']}
//					order by
//						ede.estuf, ent.entnome";
//			
//				$rsValor = $db->pegaLinha($sql);
				
				$sql = "SELECT 
							coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
						FROM 
							em.emimatrizdistribuicaoorcamentargap m
						INNER JOIN 
							em.emigap p ON p.papid = m.papid 
						WHERE 
							emiid = {$_SESSION['emiid']}  
						AND 
							itfid in (8,5,3,2,7,1,4,6,13)";
				
				$totalCusteio = $db->pegaUm($sql);
				
				$sql = "SELECT 
							coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
						FROM 
							em.emimatrizdistribuicaoorcamentargap m
						INNER JOIN 
							em.emigap p ON p.papid = m.papid
						WHERE 
							emiid = {$_SESSION['emiid']} 
						AND 
							itfid in (9,11,10,12)";
				
				$totalCapital = $db->pegaUm($sql);
				
				$saldoCusteio = $totalCusteio;
				$saldoCapital = $totalCapital;
				
				$sql = "
					SELECT 
						emvlrcusteio,
						emvlrcapital,
						emvlrpago
					FROM em.emipagtofnde
					WHERE entcodent = (select entcodent from entidade.entidade where entid = {$_SESSION['entid']})
				";
				
				$dadosMEC = $db->pegaLinha($sql);
				
				$capitalDisponivel = $dadosMEC['emvlrcapital'] - $saldoCapital;
				$custeioDisponivel = $dadosMEC['emvlrcusteio'] - $saldoCusteio;
				//$totalDisponivel   = $dadosMEC['emvlrpago']    - $totalGeralAxB;
				$totalDisponivel   = $dadosMEC['emvlrpago']    - ($saldoCapital + $saldoCusteio);
				
			?>	
			<?php //if($rsValor['itens'] > $rsValor['recurso']): ?>
			<?php if($totalDisponivel<0): ?>
				<tr>
					<td class="subtituloEsquerda">Valor total dos itens financiáveis maior que o recurso</td>
					<td>Verificar preenchimento</td>
				</tr>
			<?php endif; ?>	
			<tr>
				<td colspan="2" class="subtituloCentro"><input type="button" value="Fechar" onclick="window.close();" /></td>
			</tr>
		</table>
	</body>
</html>