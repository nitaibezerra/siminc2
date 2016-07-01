<?php

	#FUNÇÕES ORDENADAS POR ORDEM ALFABETICA.
	function formata_valor_sql($valor){
		$valor = str_replace('.', '', $valor);
		$valor = str_replace(',', '.', $valor);
		return $valor;
	}

	#MONTA A TABELA DOS ITENS DE DESPESAS. 
	function listaItensDespesas($unicod){
		global $db;
		
		$sql = "
			Select	ds.dpsid,
                                ds.dpsdescricao,
                                Case When ls.lcsvalorempenhado > 1
                                        Then trim(to_char(ls.lcsvalorempenhado, '999G999G999G999G999D99')) 
                                        Else trim(to_char(ls.lcsvalorempenhado, '0D99'))
                                End as lcsvalorempenhado,
                                replace(cast(ls.lcsvalormeta as text), '.', ',') as lcsvalormeta,
                                Case When ls.lcsvalordeducao > 1
                                        Then trim(to_char(ls.lcsvalordeducao, '999G999G999G999G999D99' ) ) 
                                        Else trim(to_char(ls.lcsvalordeducao, '0D99' ) )
                                End as lcsvalordeducao,
                                --Totais
                                trim(to_char(t.totalvalorempenhado, '999G999G999G999G999D99')) as totalvalorempenhado,
                                --trim(to_char(t.totalvalormeta, '99D99')) as totalvalormeta,
                                replace(cast(t.totalvalormeta as text), '.', ',') as totalvalormeta,
                                trim(to_char(t.totalvalordeducao, '999G999G999G999G999D99')) as totalvalordeducao
			From elabrev.despesasustentavel ds
				
			Join (
				Select	unicod,
						sum(lcsvalorempenhado) as totalvalorempenhado,
						( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) as totalvalormeta,
						sum(lcsvalordeducao) as totalvalordeducao
				From elabrev.lancamentosustentavel
				Where lcsstatus = 'A' AND lcsanoexercicio = '".$_SESSION['exercicio']."' 
				Group by unicod
			) t on  t.unicod = '".$unicod."'
				
			Left Join (
				Select	dpsid,
						unicod,
						entid,
						lcsvalorempenhado,
						lcsvalormeta,
						lcsvalordeducao
				From elabrev.lancamentosustentavel
				Where lcsstatus = 'A' AND lcsanoexercicio = '".$_SESSION['exercicio']."' 
			) ls on ls.dpsid = ds.dpsid and ls.unicod = '".$unicod."'
				
			Where ds.dpsstatus = 'A'
			Order by dpsid
		";
		$dados = $db->carregar($sql);
		
		if($dados != ''){
			foreach($dados as $dado){
				echo "<tr id=\"tr_".$dado['dpsid']."\">";
				echo "<td>
						<img border=\"0\" style=\"cursor: pointer;\" src=\"../imagens/setavd.gif\" onmouseout=\"titleNaturezaInfoHidden();\"onmouseover=\"titleNaturezaInfoVisibilyt('".$dado['dpsid']."');\">
						".$dado['dpsdescricao']."
					</td>";
		
				echo "<td style=\"text-align: right;\"><input type=\"hidden\" id=\"dpsid\" name=\"dpsid[]\" value=\"".$dado['dpsid']."\" />".
						campo_texto('lcsvalorempenhado[]', 'N', 'N', '', 42, 16, '','', 'right', '', '', 'id='.lcsvalorempenhado.'_'.$dado['dpsid'], '', $dado['lcsvalorempenhado'])
						."</td>";
					
				echo "<td style=\"text-align: right;\">".
						campo_texto('lcsvalormeta[]', 'S', 'S', '', 13, 6, '[#],##','', 'right', '', '', 'id='.lcsvalormeta.'_'.$dado['dpsid'], '', $dado['lcsvalormeta'])
						."</td>";
					
				echo "<td style=\"text-align: right;\">".
						campo_texto('lcsvalordeducao[]', 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id='.lcsvalordeducao.'_'.$dado['dpsid'], '', $dado['lcsvalordeducao'])
						."</td>";
				echo "</tr>";
			}
		}else{
			echo "
			<script>
				alert('Esta unidade não contém dados!');
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/listaUnidades&acao=A&';
			</script>
			";
		}
		?>
			<tr>
			    <th style="text-align: left; font-size: 13px;">Total</th>
			    <th style="text-align: right;"><?=campo_texto(totalvalorempenhado, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="totalvalorempenhado"', '', $dado['totalvalorempenhado'])?></th>
			    <th style="text-align: right;"><?=campo_texto(totalvalormeta, 'S', 'N', '', 13, 5, '','', 'right', '', '', 'id="totalvalormeta"', '', $dado['totalvalormeta'])?></th>
			    <th style="text-align: right;"><?=campo_texto(totalvalordeducao, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="totalvalordeducao"', '', $dado['totalvalordeducao'])?></th>
			</tr>
<?php 
	}
	
	#LISTAGEM DAS UNIDADES - MOSTRADAS AO SUPER USUÁRIO
	function listaUnidades($where){
		global $db;
			
		$sql = "
			Select 	distinct u.unicod, 
					Case When (u.unicod <> '' and us.unscodigo is not null)
						Then '<a class=\"listaUnidades\" id=\"'|| u.unicod ||'\" name=\"'|| u.unidsc ||'\" style=\"color:blue;cursor:pointer\">'|| UPPER(u.unidsc) || '</a>'
						Else '<a class=\"listaUnidadesAlunos\" id=\"'|| u.unicod ||'\" name=\"'|| u.unidsc ||'\" style=\"color:blue;cursor:pointer\">'|| UPPER(u.unidsc) || '</a>'
					End as unidade_descricao,
					'' as uniabrev
			From public.unidade u 
			Join elabrev.unidadeordenadora o on o.unicod = u.unicod 
			Left Join elabrev.unidadesustentavel us on cast(us.unscodigo as text) = u.unicod
			Where unistatus = 'A' ".$where."
			and o.uniordstatus = 'A'
			Order by 1
		";
		$cabecalho = Array("Unidade Orçamentária", "Descrição", "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;");
		$whidth = Array('20%', '60%', '20%');
		$align  = Array('left', 'left', 'center');	
		$db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
	}

	#NOTA DE RODA PÉ.
	function notaRodaPe($unicod){
		global $db;
		
		$unid_setec = array(26256, 26257, 26402, 26403, 26404, 26405, 26406, 26407, 26408, 26409, 26410, 26411, 26412, 26413, 26414, 26415, 26416, 26417, 26418, 26419, 26420, 26421, 26422, 26423, 26424, 26425, 26426, 26427, 26428, 26429, 26430, 26431, 26432, 26433, 26434, 26435, 26436, 26437, 26438, 26439,26201);
		
		$unid_sesu = array(26230, 26231, 26232, 26233, 26234, 26235, 26236, 26237, 26238, 26239, 26240, 26241, 26242, 26243, 26244, 26245, 26246, 26247, 26248, 26249, 26250, 26251, 26252, 26253, 26254, 26255, 26258, 26260, 26261, 26262, 26263, 26264, 26266, 26267, 26268, 26269, 26270, 26271, 26272, 26273, 26274, 26275, 26276, 26277, 26278, 26279, 26280, 26281, 26282, 26283, 26284, 26285, 26286, 26350, 26351, 26352, 26440, 26441, 26442);
		
		if( in_array($unicod, $unid_setec) ){
			$msg = "- Alunos Equivalentes extraídos do SISTEC (segundo semestre de 2012), Fonte: SETEC.";
		}elseif( in_array($unicod, $unid_sesu) ){
			$msg = "-   Aluno Equivalente utilizado para a elaboração do Orçamento de 2013, Fonte: SESU.";
		}else{
			$msg = "";
		}
		
		return $msg;
	}
	
	#SALVA DADOS DO FORMULÁRIO DE "PROJETO ESPLANADA SUSTENTÁVEL"
	function salvarPacto($request){
		global $db;
		
		extract( $request );
		
		if( is_array($request) ){
			$a = 0;
			foreach( $dpsid as $k){
				if( $dpsid[$a] != '' && $lcsvalormeta[$a] != ''){
					$sql .= "
						Update elabrev.lancamentosustentavel Set
							lcsvalormeta 	= ".formata_valor_sql($lcsvalormeta[$a]).",
							lcsvalordeducao = ".formata_valor_sql($lcsvalordeducao[$a]).",
							lcsobservacao   = '".$lcsobservacao."'
						Where unicod = '".$request['unicod']."' and dpsid = ".$k." and lcsanoexercicio = '".$_SESSION['exercicio']."'
                                                returning lcsid;
					";
				}
				$a = $a + 1;
			}
		}else{
			echo "<script>
			alert('Ocorreu um erro, verifique os dados informados e tente novamente mais tarde');
			window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A&unicod=".$request['unicod']."'
			</script>";
		}

		$sql_desc = "Select unidsc From public.unidade where unicod = '".$unicod."'";
		$descricao = $db->pegaUm($sql_desc);

		if( $db->executar($sql) ){
			$db->commit();
			
			criaDocidEsplanada($unicod, 'N_ALUNOS');
			
			echo "<script>
			alert('Dados gravados com sucesso!');
			window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A&unicod=".$request['unicod']."&descricao=".$descricao."'
			</script>";
		}else{
			echo "<script>
			alert('Ocorreu erro ao gravar os dados tente novamente mais tarde');
			window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A&unicod=".$request['unicod']."'
			</script>";
		}
	}	
	
	#MONTA O DIV COM INFORMAÇÕES: INTES QUE COMPÕEM "OS ITENS DE NATUREZA DE DESPESA"
	function titleNaturezaDespesa($dpsid){
		global $db;

		$sql = "
			Select 	distinct 
					d.dpsid,
					dp.dpsauxiliar,
					n.ndpcod||' - '||upper(n.ndpdsc) as ndpdsc
			From elabrev.despesanatureza d
			Join public.naturezadespesa n on n.ndpid = d.ndpid
			Join elabrev.despesasustentavel dp on dp.dpsid = d.dpsid
			Where ndpstatus = 'A' and d.dpsid = ".$dpsid['dpsid'].";";
		$dados =$db->carregar($sql);
		
		if($dados){
			$titulo .= '<table style="width: 100%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">';
			$titulo .= '<tr><th style="text-align: left;">'.$dados[0]['dpsauxiliar'].'</th></tr>';
			$x = 0;						
			foreach($dados as $dado){
				if( ($x % 2) == 0 ){
					$cor = "#FFFAFA";
				}else{
					$cor = "#D3D3D3";
				}
				$titulo .= '<tr style="background-color: '.$cor.';"><td>'.$dado['ndpdsc'].'</td></tr>';
				$x = $x + 1;
			}
			$titulo .= '</table>';
		}else{
			$titulo = 'Não há informações!';
		}
		echo $titulo;
	}

	#MOSTRA O VALOR DE 10% CALCULADO SOBRE O VALOR GLOBAL - SUB TITULO DO FORMULÁRIO / ACRESENTADO O CAMPO OBSERVAÇÃO. 
	function valorPercentualTitulo($unicod){
		global $db;
		$sql = "
			Select	unicod,
					max(lcsobservacao) as lcsobservacao,
					trim(to_char( (sum(lcsvalorempenhado) * 0.10), '999G999G999G999G999D99' ) ) as total
			From elabrev.lancamentosustentavel
			where unicod = '".$unicod."' and lcsstatus = 'A' AND lcsanoexercicio = '".$_SESSION['exercicio']."' 
			Group by unicod, lcsobservacao
		";
		$dados = $db->pegaLinha($sql);
		return $dados; 
	}
	
	#BUSCA O CAMPO OBSERVÇÃO DO FORMULARIO. 
	function buscaResObservacao($unicod){
		global $db;
		$sql = "
			Select	*
			From elabrev.lancamentosustentavelaluno
			where unicod = '".$unicod."' and lsastatus = 'A' AND lsaanoexercicio = '".$_SESSION['exercicio']."' 
		";
		$dados = $db->pegaLinha($sql);
		return $dados; 
	}
	
	
	## - FUNÇÕES RELACIONADAS COM O FORMULARIO ESPLANADA SUSTENTAVEL - ALUNOS - ##
	
	#MONTA A TABELA DOS ITENS DE DESPESAS.
	function listaItensDespesasaAlunos($unicod){
		global $db, $habilita;
	
		$sql = "
		Select	ds.dpsid,
				ds.dpsdescricao,
				
				Case When lsa.lsavalorempenhado > 1 
					Then trim(to_char(lsa.lsavalorempenhado, '999G999G999G999G999D99')) 
					Else trim(to_char(lsa.lsavalorempenhado, '0D99'))
				End as lsavalorempenhado,
				
				trim(replace(to_char(lsa.lsaalunoequivalente, '9999999999G999'), ',', '.')) as lsaalunoequivalente,
				
				Case When ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ) > 1
					Then trim( to_char( ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ), '999G999G999G999G999D99' ) ) 
					Else trim( to_char( ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ), '0D99' ) ) 
				End as lsavaloralunoequivalente,
				
				replace( cast( (lsa.lsavalorempenhado/lsa.lsaalunoequivalente ) as text), '.', ',') as h_lsavaloralunoequivalente,
				--percentual %				
				--replace(cast(round((lsametavalorreducaoaluno * 100)/lsavaloralunoequivalente, 2) as text), '.',',') as  lsametareducaoaluno,
				
				Case When substr(cast(lsavaloralunoequivalente as text), 1, 4) <> '0.00' 
					Then replace(cast(round((lsametavalorreducaoaluno * 100)/lsavaloralunoequivalente, 2) as text), '.',',') 
					Else replace(substr(cast(lsavaloralunoequivalente as text), 1, 4), '.', ',')
				End as  lsametareducaoaluno,
				
				replace(cast(lsa.lsametareducaoaluno as text), '.', ',') as h_lsametareducaoaluno,
				
				Case When lsa.lsametavalorreducaoaluno > 1
					Then trim(to_char(lsa.lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsa.lsametavalorreducaoaluno, '0D99' ) )
				End as lsametavalorreducaoaluno,
				
				replace( cast( lsa.lsametavalorreducaoaluno as text), '.', ',') as h_lsametavalorreducaoaluno,
				--totais meta economia proposta
				Case When (lsaalunoequivalente * lsametavalorreducaoaluno) > 1
					Then trim(to_char(lsaalunoequivalente * lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsaalunoequivalente * lsametavalorreducaoaluno, '0D99' ) )
				End as valortotal,
				
				Case When (lsavaloralunoequivalente - lsametavalorreducaoaluno) > 1 
					Then trim(to_char(lsavaloralunoequivalente - lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsavaloralunoequivalente - lsametavalorreducaoaluno, '0D99' ) )
				End as desppactuadaequival,
				
				--Totais
				trim(to_char(t.total_valorempenhado, '999G999G999G999G999D99')) as total_valorempenhado,
				t.total_alunoequivalente,
				trim(to_char(t.total_valoralunoequivalente, '999G999G999G999G999D99')) as total_valoralunoequivalente,
				replace(cast(t.total_metareducaoaluno as text), '.', ',') as total_metareducaoaluno,
				trim(to_char(t.total_metavalorreducaoaluno, '999G999G999G999G999D99')) as total_metavalorreducaoaluno,
				trim(to_char(t.total_valortotal, '999G999G999G999G999D99')) as total_valortotal,
				trim(to_char(t.total_desppactuadaequival, '999G999G999G999G999D99')) as total_desppactuadaequival 
		From elabrev.despesasustentavel ds
	
		Join (
			Select	unicod, 
					lsastatus,
					sum(lsavalorempenhado) as total_valorempenhado,
					sum(lsaalunoequivalente) as total_alunoequivalente,
					sum(round( (lsavalorempenhado/lsaalunoequivalente), 2) ) as total_valoralunoequivalente,
					( (  sum(lsaalunoequivalente * lsametavalorreducaoaluno) / sum(lsavalorempenhado) ) *100 ) as total_metareducaoaluno,
					sum(lsametavalorreducaoaluno) as total_metavalorreducaoaluno,
					sum(lsaalunoequivalente * lsametavalorreducaoaluno) as total_valortotal,
					sum(lsavaloralunoequivalente - lsametavalorreducaoaluno) as total_desppactuadaequival
			From elabrev.lancamentosustentavelaluno
			Where lsastatus = 'A' AND lsaanoexercicio = '".$_SESSION['exercicio']."' 
			Group by unicod, lsastatus
		) t on  t.unicod = '".$unicod."'
	
		Left Join (
			Select	dpsid,
					unicod,
					entid,
					lsastatus,
					lsavalorempenhado,
					lsaalunoequivalente,
					lsavaloralunoequivalente,
					lsametareducaoaluno,
					lsametavalorreducaoaluno
			From elabrev.lancamentosustentavelaluno
			Where lsastatus = 'A' AND lsaanoexercicio = '".$_SESSION['exercicio']."'
		) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '".$unicod."'
	
		Where ds.dpsstatus = 'A' and (t.lsastatus = 'A' and lsa.lsastatus = 'A')
		Order by dpsid
		";
		$dados = $db->carregar($sql);
		
		if($dados != ''){
			foreach($dados as $dado){
				echo "<tr id=\"tr_".$dado['dpsid']."\">";
					echo "<td>
							<img border=\"0\" style=\"cursor: pointer;\" src=\"../imagens/setavd.gif\" onmouseout=\"titleNaturezaInfoHidden();\"onmouseover=\"titleNaturezaInfoVisibilyt('".$dado['dpsid']."');\">
							".$dado['dpsdescricao']."
						</td>";
		
					echo "<td style=\"text-align: right;\">
							<input type=\"hidden\" id=\"dpsid\" name=\"dpsid[]\" value=\"".$dado['dpsid']."\" />".
							campo_texto('lsavalorempenhado[]', 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id='.lsavalorempenhado.'_'.$dado['dpsid'], '', $dado['lsavalorempenhado'])
						."</td>";
					
					echo "<td style=\"text-align: center;\">".
							campo_texto('lsaalunoequivalente[]', 'N', 'N', '', 8, 16, '','', 'center', '', '', 'id='.lsaalunoequivalente.'_'.$dado['dpsid'], '', $dado['lsaalunoequivalente'])
						."</td>";
					
					#Despesa por Aluno Equivalente - lsavaloralunoequivalente numeric(30,15),
					echo "<td style=\"text-align: right;\">
							<input type=\"hidden\" id=\"".h_lsavaloralunoequivalente."_".$dado['dpsid']."\" name=\"h_lsavaloralunoequivalente[]\" value=\"".$dado['h_lsavaloralunoequivalente']."\" />".
							campo_texto('lsavaloralunoequivalente[]', 'N', 'N', '', 15, 16, '[.###],##','', 'right', '', '', 'id='.lsavaloralunoequivalente.'_'.$dado['dpsid'], '', $dado['lsavaloralunoequivalente'])
						."</td>";
					
					#lsametareducaoaluno numeric(9,15)				
					echo "<td style=\"text-align: right;\">
							<input type=\"hidden\" id=\"".h_lsametareducaoaluno."_".$dado['dpsid']."\" name=\"h_lsametareducaoaluno[]\" value=\"".$dado['h_lsametareducaoaluno']."\" />".
							campo_texto('lsametareducaoaluno[]', 'N', 'S', '', 14, 6, '[#],##','', 'right', '', '', 'id='.lsametareducaoaluno.'_'.$dado['dpsid'], '', $dado['lsametareducaoaluno'])
						."</td>";
	
					#lsametavalorreducaoaluno numeric(30,15),
					echo "<td style=\"text-align: right;\">
							<input type=\"hidden\" id=\"".h_lsametavalorreducaoaluno."_".$dado['dpsid']."\" name=\"h_lsametavalorreducaoaluno[]\" value=\"".$dado['h_lsametavalorreducaoaluno']."\" />".
							campo_texto('lsametavalorreducaoaluno[]', 'N', 'S', '', 14, 16, '[.###],##','', 'right', '', '', 'id='.lsametavalorreducaoaluno.'_'.$dado['dpsid'], '', $dado['lsametavalorreducaoaluno'])
						."</td>";
					
					#Valor Total
					echo "<td style=\"text-align: right;\">
							<input type=\"hidden\" id=\"".h_valortotal."_".$dado['dpsid']."\" name=\"h_valortotal[]\" value=\"".$dado['valortotal']."\" />".					
							campo_texto('valortotal[]', 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id='.valortotal.'_'.$dado['dpsid'], '', $dado['valortotal'])
						."</td>";

					#Despesa 2013 pactuada por aluno equivalente
					echo "<td style=\"text-align: right;\">".
							campo_texto('desppactuadaequival[]', 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id='.desppactuadaequival.'_'.$dado['dpsid'], '', $dado['desppactuadaequival'])
							."</td>";
				
				echo "</tr>";
			}
		}else{
			echo "
			<script>
				alert('Esta unidade não contém dados!');
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/listaUnidades&acao=A&';
			</script>
			";
		}
		?>
			<tr>
			    <th style="text-align: left; font-size: 13px;">Total</th>
			    <th style="text-align: right;"><?=campo_texto(total_valorempenhado, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="total_valorempenhado"', '', $dado['total_valorempenhado'])?></th>
			    <th style="text-align: center;"><?=campo_texto(total_alunoequivalente, 'N', 'N', '', 8, 16, '','', 'center', '', '', 'id="total_alunoequivalente"', '', $dado['lsaalunoequivalente'])?></th>
			    <th style="text-align: right;"><?=campo_texto(total_valoralunoequivalente, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="total_valoralunoequivalente"', '', $dado['total_valoralunoequivalente'])?></th>
			    <th style="text-align: right;"><?=campo_texto(total_metareducaoaluno, 'S', 'N', '', 14, 16, '','', 'right', '', '', 'id="total_metareducaoaluno"', '', $dado['total_metareducaoaluno'])?></th>	    
			    <th style="text-align: right;"><?=campo_texto(total_metavalorreducaoaluno, 'S', 'N', '', 14, 16, '','', 'right', '', '', 'id="total_metavalorreducaoaluno"', '', $dado['total_metavalorreducaoaluno'])?></th>
			    <th style="text-align: right;">
			    	<input type="hidden" id="h_total_valortotal" name="h_total_valortotal" value="<?=$dado['total_valortotal']?>" />
			    	<?=campo_texto(total_valortotal, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="total_valortotal"', '', $dado['total_valortotal'])?>
			    </th>
			    <th style="text-align: right;"><?=campo_texto(total_desppactuadaequival, 'N', 'N', '', 15, 16, '','', 'right', '', '', 'id="total_desppactuadaequival"', '', $dado['total_desppactuadaequival'])?></th>
			</tr>
	<?php 
                //dbg(simec_htmlentities($sql));
        }
	
	#SALVA DADOS DO FORMULÁRIO DE "PROJETO ESPLANADA SUSTENTÁVEL - PACTO POR ALUNOS"
	function salvarPactoAluno($request) {
		global $db;

		extract( $request );

		if (is_array($request)) {
			$a = 0;
			foreach ($dpsid as $k) {
				if ($dpsid[$a] != '' && $h_lsametareducaoaluno[$a] != '') {
					$sql .= "Update elabrev.lancamentosustentavelaluno Set
                                                        lsavaloralunoequivalente = ".formata_valor_sql($h_lsavaloralunoequivalente[$a]).",
                                                        lsametareducaoaluno 	 = ".formata_valor_sql($h_lsametareducaoaluno[$a]).",
                                                        lsametavalorreducaoaluno = ".formata_valor_sql($h_lsametavalorreducaoaluno[$a]).",
                                                        lsaobservacao		 = '".$lsaobservacao."'
                                                Where 
                                                    unicod = '".$request['unicod']."' and dpsid = ".$k." and lsaanoexercicio = '".$_SESSION['exercicio']."'
                                                returning lsaid;";
				}
				$a = $a + 1;
			}
		} else {
			echo "<script>
			alert('Ocorreu um erro, verifique os dados informados e tente novamente mais tarde');
			window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunosA&acao=A&unicod=".$request['unicod']."'
			</script>";
		}

		$sql_desc = "Select unidsc From public.unidade where unicod = '".$unicod."'";
		$descricao = $db->pegaUm($sql_desc);

		if ($db->executar($sql)) {
			$db->commit();
			
			criaDocidEsplanada($request['unicod'], 'ALUNOS');
			
			echo "<script>
				alert('Dados gravados com sucesso!');
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunos&acao=A&unicod=".$request['unicod']."&descricao=".$descricao."'
			</script>";
		} else {
			echo "<script>
				alert('Ocorreu erro ao gravar os dados tente novamente mais tarde');
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunos&acao=A&unicod=".$request['unicod']."'
			</script>";
		}
	}

	#REGRAS WORKFLOW - CRIA O DOCUMENTO CASO NÃO EXISTA.
	function criaDocidEsplanada($unicod, $tcpid){
		global $db;
		
		require_once APPRAIZ ."includes/workflow.php";
		
		$usucpf = $_SESSION['usucpf'];
		
		$existeDocid = buscarDocidEsplanada( $unicod );

		if($existeDocid == ''){
			if($tcpid == 'ALUNOS'){
				$tcpid = WF_PROJETO_ESPLANADA_SUSTENTAVEL_ALUNOS;
			}else{
				$tcpid = WF_PROJETO_ESPLANADA_SUSTENTAVEL;
			}
			
			if($unicod != '' && $tcpid != ''){
				$docid = wf_cadastrarDocumento($tcpid, 'Esplanada Sustentável');
			
				$sql = "
					Insert into elabrev.wf_esplanada_sustentavel( tcpid, docid, unicod, usucpf ) Values ( ".$tcpid.", ".$docid.", '".$unicod."', '".$usucpf."' );
				";
				
				if( $db->executar($sql) ){
					$db->commit();
				}else{
					echo "<script>
							alert('Ocorreu falha ao gravar os dados, tente mais tarde!');
							window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunos&acao=A&unicod=".$request['unicod']."&descricao=".$descricao."'
						</script>
					";
				}
			}
		}			
	}
	
	#REGRAS WORKFLOW - CRIA O DOCUMENTO CASO NÃO EXISTA.
	function buscarDocidEsplanada( $unicod ){
		global $db;			
		 
		$sql = "
			Select 	esplaid,
					tcpid,
					docid,
					unicod
			From elabrev.wf_esplanada_sustentavel
			Where unicod = '".$unicod."'
		";

		$dados = $db->pegaLinha($sql);
		
		return $dados['docid'];
	}
	
	#VALIDAR CONDIÇÃO DE AÇÃO DO WORKDLOW.
	function validaCondicao($unicod, $esplaAluno){
		global $db;

		if($esplaAluno == 'S'){
			$sql = "
				Select 	sum(lsavaloralunoequivalente) as meta,
						sum(lsametareducaoaluno) as valor
				From elabrev.lancamentosustentavelaluno
				where unicod = '".$unicod."' AND lsaanoexercicio = '".$_SESSION['exercicio']."' 
			";
		}else{
			$sql = "
				Select 	sum(lcsvalormeta) as meta,
						sum(lcsvalordeducao) as valor
				From elabrev.lancamentosustentavel
				where unicod = '".$unicod."' AND lcsanoexercicio = '".$_SESSION['exercicio']."' 
			";
		}
		$dados = $db->pegaLinha($sql);

		if( $dados['meta'] > 0 && $dados['valor'] > 0 ){
			return true;
		}else{
			return false;
		}
	}
	
	#PEGA ESTADO ATUAL DO DOCUMENTO DO WORKFLOW.
	function pegaEstadoAtualEsplanada($docid){
		global $db;
	
		if($docid) {
			$docid = (integer) $docid;
			$sql = "
				Select ed.esdid
				From workflow.documento d
				inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where d.docid = $docid
			";
			$estado = $db->pegaUm($sql);
			return $estado;
		} else {
			return false;
		}
	}
	
	#FUNÇÃO USADO PELO WORKFLOW - VERIFICA SE A UNIDADE ATINGIU A META DE 10%, CASO NÃO ABILITA O BOTÃO DO WORKFLOW PARA RETONA AO CADASTRAMENTO.
	function verificaMetaAlunos($unicod){
		global $db;

		$sql = "
			Select	l.unicod
			From elabrev.lancamentosustentavelaluno l
			join elabrev.unidadeordenadora uo on uo.unicod = l.unicod
			Where lsastatus = 'A' and uniordstatus = 'A' and l.unicod = '".$unicod."' AND l.lsaanoexercicio = '".$_SESSION['exercicio']."' 
			Group by l.unicod, uo.unidsc, lsastatus having ( (  sum(lsaalunoequivalente * lsametavalorreducaoaluno) / sum(lsavalorempenhado) ) *100 ) < 10
		";
		$estado = $db->pegaUm($sql);

		if($estado != ''){
			return true;
		}else{
			return false;
		}		
	}
		
	#FUNÇÃO USADO PELO WORKFLOW - VERIFICA SE A UNIDADE ATINGIU A META DE 10%, CASO NÃO ABILITA O BOTÃO DO WORKFLOW PARA RETONA AO CADASTRAMENTO.
	function verificaMeta($unicod){
		global $db;
		
		$sql = "
			Select	l.unicod
			From elabrev.lancamentosustentavel l
			join elabrev.unidadeordenadora uo on uo.unicod = l.unicod
			Where lcsstatus = 'A' and l.unicod = '".$unicod."' AND l.lcsanoexercicio = '".$_SESSION['exercicio']."' 
			Group by l.unicod, uo.unidsc having ( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) < 10
		";
		$estado = $db->pegaUm($sql);
		
		if($estado != ''){
			return true;
		}else{
			return false;
		}
	}