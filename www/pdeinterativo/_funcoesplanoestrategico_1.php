<?


function enviadoComite() {
	global $db;
	$msg.="O Plano de Desenvolvimento da Escola foi enviado para análise do MEC fora do prazo, que expirou no dia 09/12/2011. Acesse o site do PDE Escola no endereço http://pdeescola.mec.gov.br e conheça as regras de análise para os planos enviados fora do prazo.".'\n\n\n';	
	$msg.="O plano da (nome da escola) foi enviado para análise do Comitê. Caso o mesmo seja aprovado pela Secretaria e validado pelo MEC, a escola receberá os recursos indicados no plano mas, para que isso aconteça, confira se:".'\n\n';
	$msg.="1)	A escola possui Unidade Executora (UEx) e a mesma já se recadastrou no PDDE Web este ano;".'\n';
	$msg.="2)	A prestação de contas dos anos anteriores foi encaminhada, recebida e aprovada pelo FNDE;".'\n';
	$msg.="3)	O saldo de recursos de 2009, se houver, foi reprogramado para 2010;".'\n';
	$msg.="4)	A conta corrente da Unidade Executora (UEx) continua ativa.".'\n';
	$msg.="Se tiver dúvidas, acesse <www.fnde.gov.br>, procure o link <Consulta Prestação de Contas> e preencha os campos indicando o nome do seu município e da sua escola. Ou envie um e-mail para pdeescola@mec.gov.br.";
	echo "<script>alert('".$msg."');</script>";
	return true;
	
}

function listasGrandesDesafios($dados) {
	global $db;
	
	$sql = "SELECT * FROM pdeinterativo.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadosideb = $db->pegaLinha($sql);
	
	$filtro = $dados['filtro'];
	
	if($dadosideb['ridinicialum'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar o IDEB dos Anos Iniciais em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaideb[ridinicialum] value='".$dadosideb['ridinicialumdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos.";
	}
	if($dadosideb['ridfinalum'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar o IDEB dos Anos Finais em <input ".(($dados['desabilitar'])?"disabled":"")." type=text name=respostaideb[ridfinalum] value='".$dadosideb['ridfinalumdesafio']."' class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos.";
	}
	if($dadosideb['ridmedioum'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar o IDEB do Ensino Médio em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaideb[ridmedioum] value='".$dadosideb['ridmedioumdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos.";
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadostx = $db->pegaLinha($sql);
	
	if($dadostx['rtrfunaprova'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar a taxa de aprovação da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrfunaprova] value='".$dadostx['rtrfunaprovadesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrfunreprova'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Reduzir a taxa de reprovação da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrfunreprova] value='".$dadostx['rtrfunreprovadesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrfunabandono'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Reduzir a taxa de abandono da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrfunabandono] value='".$dadostx['rtrfunabandonodesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrmedaprova'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar a taxa de aprovação da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrmedaprova] value='".$dadostx['rtrmedaprovadesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Médio.";
	}
	if($dadostx['rtrmedreprova'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Reduzir a taxa de reprovação da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrmedreprova] value='".$dadostx['rtrmedreprovadesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Médio.";
	}
	if($dadostx['rtrmedabandono'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Reduzir a taxa de abandono da escola em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostataxarendimento[rtrmedabandono] value='".$dadostx['rtrmedabandonodesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos no Ensino Médio.";
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadospb = $db->pegaLinha($sql);
	
	if($dadospb['rpbinicialport'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar os resultados de Língua Portuguesa na Prova Brasil em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaprovabrasil[rpbinicialport] value='".$dadospb['rpbinicialportdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos nos Anos Iniciais.";
	}
	if($dadospb['rpbinicialmat'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar os resultados de Matemática na Prova Brasil em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaprovabrasil[rpbinicialmat] value='".$dadospb['rpbinicialmatdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos.";
	}
	if($dadospb['rpbfinalport'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar os resultados de Língua Portuguesa na Prova Brasil em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaprovabrasil[rpbfinalport] value='".$dadospb['rpbfinalportdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em dois anos nos Anos Finais.";
	}
	if($dadospb['rpbfinalmat'.$filtro]=="t") {
		$desafiosindicadorestaxas[] = "<img src=../imagens/seta_filho.gif> Elevar os resultados de Matemática na Prova Brasil em <input ".(($dados['desabilitar'])?"disabled":"")." name=respostaprovabrasil[rpbfinalmat] value='".$dadospb['rpbfinalmatdesafio']."' type=text class=normal size=4 maxlength=3 onkeyup=this.value=mascaraglobal('###',this.value);> % em Anos Finais.";
	}
	
	if($dados['return_arr']) {
		
		return $desafiosindicadorestaxas;
		
	} else {
	
		echo "<table class=listagem cellSpacing=1 cellPadding=3	align=center width=100%>";
		if($desafiosindicadorestaxas) {
			
			foreach($desafiosindicadorestaxas as $desafio) {
				echo "<tr>
						<td>$desafio</td>
					  </tr>";
			}
		} else {
			
			echo "<tr>
					<td align=\"center\" style=\"color: rgb(204, 0, 0);\">Não foram encontrados Registros.</td>
				  </tr>";
			
		}
		echo "</table>";
	
	}
	
}


function gerenciarOutrosDesafios($dados) {
	global $db;
	
	$sql = "SELECT * FROM pdeinterativo.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadosideb = $db->pegaLinha($sql);
	
	if($dadosideb['ridinicialumcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadosideb['ridinicialumdesafiooutros']=="t")?"checked":"")." name=ridinicialumdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaideb',this);> Elevar o IDEB dos Anos Iniciais em ### % em dois anos.";
	}
	if($dadosideb['ridfinalumcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadosideb['ridfinalumdesafiooutros']=="t")?"checked":"")." name=ridfinalumdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaideb',this);> Elevar o IDEB dos Anos Finais em ### % em dois anos.";
	}
	if($dadosideb['ridmedioumcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadosideb['ridmedioumdesafiooutros']=="t")?"checked":"")." name=ridmedioumdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaideb',this);> Elevar o IDEB do Ensino Médio em ### % em dois anos.";
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadostx = $db->pegaLinha($sql);
	
	if($dadostx['rtrfunaprovacritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrfunaprovadesafiooutros']=="t")?"checked":"")." name=rtrfunaprovadesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Elevar a taxa de aprovação da escola em ### % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrfunreprovacritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrfunreprovadesafiooutros']=="t")?"checked":"")." name=rtrfunreprovadesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Reduzir a taxa de reprovação da escola em ### % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrfunabandonocritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrfunabandonodesafiooutros']=="t")?"checked":"")." name=rtrfunabandonodesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Reduzir a taxa de abandono da escola em ### % em dois anos no Ensino Fundamental.";
	}
	if($dadostx['rtrmedaprovacritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrmedaprovadesafiooutros']=="t")?"checked":"")." name=rtrmedaprovadesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Elevar a taxa de aprovação da escola em ### % em dois anos no Ensino Médio.";
	}
	if($dadostx['rtrmedreprovacritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrmedreprovadesafiooutros']=="t")?"checked":"")." name=rtrmedreprovadesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Reduzir a taxa de reprovação da escola em ### % em dois anos no Ensino Médio.";
	}
	if($dadostx['rtrmedabandonocritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadostx['rtrmedabandonodesafiooutros']=="t")?"checked":"")." name=rtrmedabandonodesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostataxarendimento',this);> Reduzir a taxa de abandono da escola em ### % em dois anos no Ensino Médio.";
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$dadospb = $db->pegaLinha($sql);
	
	if($dadospb['rpbinicialportcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadospb['rpbinicialportdesafiooutros']=="t")?"checked":"")." name=rpbinicialportdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaprovabrasil',this);> Elevar os resultados de Língua Portuguesa na Prova Brasil em ### % em dois anos nos Anos Iniciais.";
	}
	if($dadospb['rpbinicialmatcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadospb['rpbinicialmatdesafiooutros']=="t")?"checked":"")." name=rpbinicialmatdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaprovabrasil',this);> Elevar os resultados de Matemática na Prova Brasil em ### % em dois anos.";
	}
	if($dadospb['rpbfinalportcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadospb['rpbfinalportdesafiooutros']=="t")?"checked":"")." name=rpbfinalportdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaprovabrasil',this);> Elevar os resultados de Língua Portuguesa na Prova Brasil em ### % em dois anos nos Anos Finais.";
	}
	if($dadospb['rpbfinalmatcritico']!="t") {
		$desafiosindicadorestaxas[] = "<input type=checkbox ".(($dadospb['rpbfinalmatdesafiooutros']=="t")?"checked":"")." name=rpbfinalmatdesafiooutros value=TRUE onclick=marcarOutrosDesafios('respostaprovabrasil',this);> Elevar os resultados de Matemática na Prova Brasil em ### % em Anos Finais.";
	}
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script>
			function marcarOutrosDesafios(tabela,obj) {
				jQuery.ajax({
			   		type: \"POST\",
			   		url: \"pdeinterativo.php?modulo=principal/planoestrategico&acao=A\",
			   		data: \"requisicao=marcarOutrosDesafios&tabela=\"+tabela+\"&campo=\"+obj.name+\"&marcado=\"+obj.checked,
			   		async: false,
			   		success: function(msg){}
			 		});
			 		
			 	window.opener.listaOutrosDesafios();
			}
		  </script>";
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr><td class=SubTituloCentro>Outros Desafios</td></tr>";
	
	if($desafiosindicadorestaxas) {
		
		foreach($desafiosindicadorestaxas as $desafio) {
			echo "<tr>
					<td>$desafio</td>
				  </tr>";
		}
	} else {
		
		echo "<tr>
				<td align=\"center\" style=\"color: rgb(204, 0, 0);\">Não foram encontrados Registros.</td>
			  </tr>";
		
	}
	echo "<tr><td class=SubTituloDireita><input type=button value=Ok onclick=window.close();></td></tr>";
	echo "</table>";
	
}

function marcarOutrosDesafios($dados) {
	global $db;
	
	$sql = "UPDATE pdeinterativo.".$dados['tabela']." SET ".$dados['campo']."=".$dados['marcado']." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$db->executar($sql);
	$db->commit();
	apagarCachePdeInterativo();	
	
}

function planoestrategico_0_2_planoacao($dados) {
	global $db;
	
	if(!$dados['salvarparcial']) {
		$papids = $db->carregarColuna("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papstatus='A' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		$prb_sem_estrategia = false;
		$est_sem_acao = false;
		if($papids) {
			foreach($papids as $papid) {
				$paeids = $db->carregarColuna("SELECT paeid FROM pdeinterativo.planoacaoestrategia WHERE paestatus='A' AND papid='".$papid."'");
				if($paeids) {
					foreach($paeids as $paeid) {
						$num = $db->pegaUm("SELECT COUNT(paaid) as num FROM pdeinterativo.planoacaoacao WHERE paastatus='A' AND paeid='".$paeid."'");
						if($num==0) {
							$est_sem_acao = true;
						}
					}
				} else {
					$prb_sem_estrategia = true;				
				}
			}
		}
	
		if($prb_sem_estrategia) $alert[] = "O usuário só poderá inserir no mínimo 1 (uma) e no máximo 2 (duas) estratégias para cada problema.";
		if($est_sem_acao) $alert[] = "É obrigatório inserir pelo menos uma ação para cada estratégia e, no mínimo, uma estratégia para cada problema.";
		
		if($alert) {
			die("<script>
					alert('Não foi possivel gravar o plano de ação. Os seguintes problemas foram encontrados:".'\n\n'.implode('\n',$alert)."');
					window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';
				 </script>");
		}
	}
	
	if($dados['metas']) {
		foreach($dados['metas'] as $metid => $valor) {
			$sql = "SELECT rmeid FROM pdeinterativo.respostameta WHERE metid='".$metid."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
			$rmeid = $db->pegaUm($sql);
			if($rmeid) {
				$sql = "UPDATE pdeinterativo.respostameta
   						SET rmetaxa=".(($valor)?"'".$valor."'":"NULL")."
 						WHERE rmeid='".$rmeid."'";
			} else {
				$sql = "INSERT INTO pdeinterativo.respostameta(
		            	metid, pdeid, rmetaxa)
		    			VALUES ('".$metid."', '".$_SESSION['pdeinterativo_vars']['pdeid']."', ".(($valor)?"'".$valor."'":"NULL").");";
			}
			$db->executar($sql);
			$db->commit();
		}
		
	}
	
	if($dados['metas2']) {
		foreach($dados['metas2'] as $metid => $valor) {
			$sql = "SELECT rmeid FROM pdeinterativo.respostameta WHERE metid='".$metid."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
			$rmeid = $db->pegaUm($sql);
			if($rmeid) {
				$sql = "UPDATE pdeinterativo.respostameta
   						SET rmecheckbox=".(($valor)?$valor:"NULL")."
 						WHERE rmeid='".$rmeid."'";
			} else {
				$sql = "INSERT INTO pdeinterativo.respostameta(
		            	metid, pdeid, rmecheckbox)
		    			VALUES ('".$metid."', '".$_SESSION['pdeinterativo_vars']['pdeid']."', ".(($valor)?$valor:"NULL").");";
			}
			$db->executar($sql);
			$db->commit();
		}
		
	}

	if($dados['pesid_estrategias']) {
		foreach($dados['pesid_estrategias'] as $paeid => $respid) {
			$sql = "UPDATE pdeinterativo.planoacaoestrategia SET respid='".$respid."' WHERE paeid='".$paeid."'";			
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['aoaid']) {
		foreach($dados['aoaid'] as $abaid => $aoaid) {
			$opaid = $db->pegaUm("SELECT opaid FROM pdeinterativo.objetivoplanoacao WHERE abaid='".$abaid."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if($opaid) {
				
				$sql = "UPDATE pdeinterativo.objetivoplanoacao SET aoaid=".(($aoaid)?"'".$aoaid."'":"NULL")."
 						WHERE opaid='".$opaid."'";
				
				$db->executar($sql);
				$db->commit();
				
			} else {
				
				if($aoaid) {
					$sql = "INSERT INTO pdeinterativo.objetivoplanoacao(
	            			pdeid, abaid, aoaid, opastatus)
	    					VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$abaid."', '".$aoaid."', 'A');";
					$db->executar($sql);
					$db->commit();
				}
			}
		}
	}
	
	apagarCachePdeInterativo();
	
	if(!$dados['salvarparcial']) {
		
		salvarAbaResposta("planoestrategico_0_2_planoacao");
		
		echo "<script>
				alert('Dados gravados com sucesso');
				window.location='".$dados['togo']."';
			  </script>";
	}
	
	
	
}

function planoestrategico_0_1_grandesdesafios($dados) {
	global $db;
	
	if((count($dados['respostaprovabrasil'])+count($dados['respostaideb'])+count($dados['respostataxarendimento'])) < 2) {
		echo "<script>
				alert('É obrigatório 2(dois) desafios. Clique em Inserir outros desafios.');
				window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';
			  </script>";
		exit;
		
	}
	
	if($dados['respostaideb']) {
		$ridid = $db->pegaUm("SELECT ridid FROM pdeinterativo.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		if(!$ridid) die("<script>alert('É necessário salvar o Diagnostico: 1.1. IDEB');window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';</script>");
		foreach($dados['respostaideb'] as $campo => $valor) {
			$sql = "UPDATE pdeinterativo.respostaideb SET ".$campo."desafio=".(($valor)?"'".$valor."'":"NULL")." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['respostataxarendimento']) {
		$rtrid = $db->pegaUm("SELECT rtrid FROM pdeinterativo.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		if(!$rtrid) die("<script>alert('É necessário salvar o Diagnostico: 1.2. Taxas de rendimento');window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';</script>");
		foreach($dados['respostataxarendimento'] as $campo => $valor) {
			$sql = "UPDATE pdeinterativo.respostataxarendimento SET ".$campo."desafio=".(($valor)?"'".$valor."'":"NULL")." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	if($dados['respostaprovabrasil']) {
		$rpbid = $db->pegaUm("SELECT rpbid FROM pdeinterativo.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		if(!$rpbid) die("<script>alert('É necessário salvar o Diagnostico: 1.3. Prova Brasil');window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';</script>");
		foreach($dados['respostaprovabrasil'] as $campo => $valor) {
			$sql = "UPDATE pdeinterativo.respostaprovabrasil SET ".$campo."desafio=".(($valor)?"'".$valor."'":"NULL")." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
		
	salvarAbaResposta("planoestrategico_0_1_grandesdesafios");
	
	apagarCachePdeInterativo();	
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}

function atualizarProblemasDimensao($dados) {
	global $db;
	$sql = "SELECT UPPER(abadescricao) as abadescricao, abacod, abaid FROM pdeinterativo.aba WHERE abaidpai = '".ABA_DIAGNOSTICO."' AND abaid != ".ABA_DIAGNOSTICO_TAXASINDICADORES." AND abatipo IS NULL ORDER BY abaid";
	if(CACHE_MEM) {
		$abas = $db->carregar($sql,null,3600);
	} else {
		$abas = $db->carregar($sql);
	}
	
	$sql = "UPDATE pdeinterativo.planoacaoproblema SET papstatus='I' WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$db->executar($sql);
	$db->commit();
	
	if($abas[0]) :
		foreach($abas as $aba) :
			//$inicioP = getmicrotime();
			$funcao = "listaproblemas_".$aba['abacod'];
			$problemas = $funcao($dados);
			if($problemas) :
				foreach($problemas as $problema) :
					if($problema['codigo']) $prb_codigo[] = $problema['codigo'];
				endforeach;
			endif;
			//echo "Texec - ".$aba['abacod']." : ".number_format( ( getmicrotime() - $inicioP ), 4, ',', '.' )."<br>";
		endforeach;
		
		if($prb_codigo) {
			foreach($prb_codigo as $pp) {
				$db->executar("UPDATE pdeinterativo.planoacaoproblema SET papstatus='A' WHERE papid='".$pp."'");
				$db->commit();
			}
		}
	endif;
	
}
	
function carregarPlanoEstrategicoDimensao($dados) {
	global $db;
	if(CACHE_FILE) {
		/* Início - Cache em arquivo*/
		include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
		$cache = new cache("planoestrategico_".$_SESSION['pdeinterativo_vars']['pdeid']);
		/* Fim - Cache em arquivo*/
	}
	
	if(verificaFlagPDEInterativo('atualizaplano')) {
		atualizarProblemasDimensao($dados);
		$db->executar("UPDATE pdeinterativo.flag SET atualizaplano=FALSE WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		$db->commit();
	}
	echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3	align=center>";

	$sql = "SELECT UPPER(abadescricao) as abadescricao, abacod, abaid FROM pdeinterativo.aba WHERE abaidpai = '".ABA_DIAGNOSTICO."' AND abaid != ".ABA_DIAGNOSTICO_TAXASINDICADORES." AND abatipo IS NULL ORDER BY abaid";
	if(CACHE_MEM) {
		$abas = $db->carregar($sql,null,3600);
	} else {
		$abas = $db->carregar($sql);
	}
	
	if($abas[0]) :
		foreach($abas as $aba) :
			planoestrategicoDimensao($dados = array('abaid' => $aba['abaid'], 'abadescricao' => $aba['abadescricao'],'abacod' => $aba['abacod']));
		endforeach;
	endif;
	
	$db->commit();
	
	echo "</table>";
}

function excluirAcao($dados) {
	global $db;

	apagarCachePdeInterativo();
		
	$sql = "UPDATE pdeinterativo.planoacaoacao SET paastatus='I' WHERE paaid='".$dados['paaid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Ação removida com sucesso";
}

function gerenciarEstrategias($dados) {
	global $db;

	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>
		  function marcarEstrategia(epaid,obj) {
		  	var numMarcadas = jQuery(\"[name^='estrategiaplanoacaoapoio[']:checked\").length;
			if(numMarcadas > 2) {
				alert('O usuário só poderá inserir no mínimo 1 (uma) e no máximo 2 (duas) estratégias para cada problema.');
				obj.checked=false;
				return false;
			} else {
			  	divCarregando();
				jQuery.ajax({
			   		type: \"POST\",
			   		url: \"pdeinterativo.php?modulo=principal/planoestrategico&acao=A\",
			   		data: \"requisicao=gravarEstrategiaProblema&epaid=\"+epaid+\"&papid=".$dados['papid']."&chk=\"+obj.checked,
			   		async: false,
			   		success: function(msg){}
			 		});
			 	window.opener.carregarPlanoEstrategicoDimensao();
			 	divCarregado();
		 	}
		  }
		  </script>";
	echo "<body>";
	echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
	echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT abadescricao FROM pdeinterativo.aba WHERE abaid='".$dados['abaid']."'")."</td></tr>";
	echo "</table>";
	
	$sql = "SELECT '<input '||CASE WHEN (SELECT paeid FROM pdeinterativo.planoacaoestrategia WHERE papid='".$dados['papid']."' AND epaid=e.epaid AND paestatus='A' ORDER BY paeid DESC LIMIT 1 ) IS NULL THEN '' ELSE 'checked' END ||' type=checkbox name=estrategiaplanoacaoapoio[] value=TRUE onclick=\"marcarEstrategia(\''||epaid||'\', this);\">' as chk, epadesc FROM pdeinterativo.estrategiaplanoacaoapoio e WHERE abaid='".$dados['abaid']."' AND epastatus='A'";
	$db->monta_lista_simples($sql,$cabecalho=array("&nbsp;","Estratégia"),50,5,'N','95%',$par2);
	echo "</body>";

	
}

function gravarEstrategiaProblema($dados) {
	global $db;

	$sql = "SELECT paeid FROM pdeinterativo.planoacaoestrategia WHERE papid='".$dados['papid']."' AND epaid='".$dados['epaid']."'";
	$paeid = $db->pegaUm($sql);
	
	if($dados['chk']=='true' && !$paeid) {
		$sql = "INSERT INTO pdeinterativo.planoacaoestrategia(
			            papid, epaid, paestatus)
			    VALUES ('".$dados['papid']."', '".$dados['epaid']."', 'A');";
		$db->executar($sql);
	} elseif($dados['chk']=='false' && $paeid) {
		$sql = "UPDATE pdeinterativo.planoacaoestrategia SET paestatus='I' WHERE paeid='".$paeid."'";
		$db->executar($sql);
	} elseif($dados['chk']=='true' && $paeid) {
		$sql = "UPDATE pdeinterativo.planoacaoestrategia SET paestatus='A' WHERE paeid='".$paeid."'";
		$db->executar($sql);
	}
	$db->commit();
	apagarCachePdeInterativo();
	
	
}

function deletarBensServicos($dados) {
	global $db;
	if(is_numeric($dados['pabid'])) {
		$sql = "UPDATE pdeinterativo.planoacaobemservico SET pabstatus='I' WHERE pabid='".$dados['pabid']."'";
		$db->executar($sql);
		$db->commit();
		$sql = "UPDATE pdeinterativo.planoacaoacao SET paacustototal=(SELECT SUM(COALESCE(pabvalorcapital,0)+COALESCE(pabvalorcusteiro,0)) FROM pdeinterativo.planoacaobemservico WHERE pabstatus='A' AND paaid IN(SELECT paaid FROM pdeinterativo.planoacaobemservico WHERE pabid='".$dados['pabid']."' AND pabstatus='A')) 
				WHERE paaid IN(SELECT paaid FROM pdeinterativo.planoacaobemservico WHERE pabid='".$dados['pabid']."' AND pabstatus='A')";
			
		$db->executar($sql);
		$db->commit();
		echo "Deletado com sucesso";
		apagarCachePdeInterativo();
	} else {
		echo "Não foi possível remover 'Bens e Serviços'";
	}
	
}

function gerenciarAcao($dados) {
	global $db;
	$sql_acao = "SELECT aapid as codigo, aapdesc as descricao FROM pdeinterativo.acaoapoio WHERE aapstatus='A'";
	$sql_objeto = "SELECT oapid as codigo, oapdesc as descricao FROM pdeinterativo.objetoapoio WHERE oapstatus='A'";
	?>
	<html>
	<head>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
	<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (BOX) -->
	<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
	<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	</head>
	<body>
	<? 
	if($dados['paaid']) {
		
		if(!is_numeric($dados['paaid'])) die("<script>alert('Problemas para abrir ação. Tente novamente.');window.close();</script>");
		
		$sql = "SELECT * FROM pdeinterativo.planoacaoacao WHERE paaid='".$dados['paaid']."'";
		$planoacaoacao = $db->pegaLinha($sql);
		
		extract($planoacaoacao);
		
		$requisicao = "atualizarAcaoPlanoEstrategico";
			
	} else {
		
		$requisicao = "inserirAcaoPlanoEstrategico";
		
	}
	?>
	<script>
	function submeterAcao() {
		if(document.getElementById('aapid').value=='') {
			alert('Selecione uma ação');
			return false;
		}
		document.getElementById('paaacaoqtd').value = mascaraglobal('#######',document.getElementById('paaacaoqtd').value);
		if(document.getElementById('paaacaoqtd').value=='') {
			alert('Preencha a quantidade');
			return false;
		}
		if(document.getElementById('oapid').value=='') {
			alert('Selecione um objeto');
			return false;
		}
		if(document.getElementById('paaperiodoinicio').value=='') {
			alert('Preencha a data inicial');
			return false;
		}
		if(!validaData(document.getElementById('paaperiodoinicio'))) {
			alert('Data inicial inválida');
			return false;
		}
		if(document.getElementById('paaperiodofim').value=='') {
			alert('Preencha a data final');
			return false;
		}
		if(!validaData(document.getElementById('paaperiodofim'))) {
			alert('Data final inválida');
			return false;
		}
		if(document.getElementById('paadetalhamento').value=='') {
			alert('Preencha o detalhamento');
			return false;
		}
		if(jQuery("[name^='paarecurso']:checked").length == 0) {
			alert('Selecione se precisa de recurso');
			return false;
		}
		
		divCarregando();
		document.getElementById('formulario').submit();

		
	}
	
	function deletarBensServicos(pabid) {
		var conf = confirm('Deseja realmente excluir?');
		if(conf) {
			divCarregando();
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=deletarBensServicos&pabid="+pabid,
		   		async: false,
		   		success: function(msg){
				alert(msg);
				carregarBensServicos();
				carregarSaldoPdeEscolaAcao();
				}
		 		});
		 	divCarregado();
		}
	}
	
	function carregarBensServicos() {
		var paaid = jQuery("[name='paaid']").val();
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
	   		data: "requisicao=carregarBensServicos&paaid=" + paaid,
	   		async: false,
	   		success: function(msg){document.getElementById('div_bensservicos').innerHTML = msg;}
	 		});
	}
	
	function carregarSaldoPdeEscolaAcao() {
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
	   		data: "requisicao=carregarSaldoPdeEscolaAcao&paaid="+jQuery("[name='paaid']").val(),
	   		async: false,
	   		success: function(msg){document.getElementById('td_saldoPdeEscolaAcao').innerHTML = msg;}
	 		});
	}
	
	function exibePrecisaRecursos(resp)
	{
		jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","disabled");
		var paaid = jQuery("[name='paaid']").val();
		
		if(!paaid){
			var msg='';
			var erro_formulario=false;
			
			if(document.getElementById('paaperiodoinicio').value!='') {
				if(!validaData(document.getElementById('paaperiodoinicio'))) {
					msg+='- Data inicial inválida\n';			
					erro_formulario=true;
				}
			}
			if(document.getElementById('paaperiodofim').value!='') {
				if(!validaData(document.getElementById('paaperiodofim'))) {
					msg+='- Data final inválida\n';
					erro_formulario=true;
				}
			}
			
			if(erro_formulario) {
				alert(msg);
				document.getElementsByName('paarecurso')[0].checked=false;
				document.getElementsByName('paarecurso')[1].checked=false;
				jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
				return false;
			}
			
			paaid = salvaAcaoAjax();
			jQuery("[name='requisicao']").val('atualizarAcaoPlanoEstrategico');
			jQuery("[name='paaid']").val(paaid);
		}
		
		if(resp == true){
			var erro_formulario=false;
			var msg='Foram encontrados erros:\n\n';
			if(document.getElementById('aapid').value=='') {
				msg+='- Selecione uma ação\n'; 
				erro_formulario=true;
			}
			if(document.getElementById('paaacaoqtd').value=='') {
				msg+='- Preencha a quantidade\n';
				erro_formulario=true;
			}
			if(document.getElementById('oapid').value=='') {
				msg+='- Selecione um objeto\n';
				erro_formulario=true;
			}
			if(document.getElementById('paaperiodoinicio').value=='') {
				msg+='- Preencha a data inicial\n';
				erro_formulario=true;
			}
			if(!validaData(document.getElementById('paaperiodoinicio'))) {
				msg+='- Data inicial inválida\n';			
				erro_formulario=true;
			}
			if(document.getElementById('paaperiodofim').value=='') {
				msg+='- Preencha a data final\n';
				erro_formulario=true;
			}
			if(!validaData(document.getElementById('paaperiodofim'))) {
				msg+='- Data final inválida\n';
				erro_formulario=true;
			}
			if(document.getElementById('paadetalhamento').value=='') {
				msg+='- Preencha o detalhamento\n';
				erro_formulario=true;
			}
			if(erro_formulario) {
				alert(msg);
				document.getElementsByName('paarecurso')[0].checked=false;
				document.getElementsByName('paarecurso')[1].checked=false;
				jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
				return false;
			}
		
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=carregarBensServicos&paaid=" + paaid,
		   		async: false,
		   		success: function(msg){
		   			jQuery("#tr_bens_servicos").show();
		   			jQuery("#div_bensservicos").html(msg);
		   			jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
		   		}
		 		});
		}else{
			if(verificaBensServicos(paaid)){
				if(confirm("Existem Aquisições e Contratações vinculadas a esta Ação, deseja excluí-las?")){
					excluirBensServicosPorAcao(paaid);
					jQuery("#tr_bens_servicos").hide();
					jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
				}else{
					jQuery("[name='paarecurso'][value='TRUE']").attr("checked","checked");
					jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
				}
			}else{
				jQuery("#div_bensservicos").html("");
				jQuery("#tr_bens_servicos").hide();
				jQuery("[name='btn_salvar'],[name='btn_cancelar']").attr("disabled","");
			}
		}
	}
	
	function salvaAcaoAjax()
	{
		var paaid = '';
		
		document.getElementById('paaacaoqtd').value=mascaraglobal('#######',document.getElementById('paaacaoqtd').value);
		
		var arrDados = jQuery("#formulario").serialize();
		jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: arrDados + "&requisicao=salvaAcaoAjax",
		   		async: false,
		   		success: function(msg){
		   			paaid = msg;
		   			}
		 		});
		 		return paaid;
	}
	
	function verificaBensServicos(paaid)
	{
		var count = false;
		jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=verificaBensServicos&paaid=" + paaid,
		   		async: false,
		   		success: function(msg){
		   			count = msg;
		   			}
		 		});
		 		
		 		if(count){
		 			return true;
		 		}else{
		 			return false;
		 		}
	}
	
	function excluirBensServicosPorAcao(paaid)
	{
		jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=excluirBensServicosPorAcao&paaid=" + paaid,
		   		async: false,
		   		success: function(msg){
		   			jQuery("#div_bensservicos").html("");
		   			}
		 		});
	}
	
	jQuery(document).ready(function() {
		carregarSaldoPdeEscolaAcao();
	<? if($paaid): ?>
		carregarBensServicos();
	<? endif; ?>
	});
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		var today = new Date();
		messageObj.setSource(url+'&hash='+today);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage() {
		messageObj.close();	
	}
	
	function inserirBens()
	{
		var today = new Date();
		var paaid = jQuery("[name='paaid']").val();
		displayMessage('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarBensServicos&paaid='+paaid+'&hash='+today);
	}
	</script>
	<form method="post" id="formulario">
	<input type="hidden" name="paeid" value="<?=$dados['paeid'] ?>">
	<input type="hidden" name="paaid" value="<?=$dados['paaid'] ?>">
	<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita"><font class="blue">Orientações:</font></td>
			<td class="blue">
		<p>Para facilitar a definição das ações, o MEC selecionou previamente alguns verbos de ação e os principais objetos para cada verbo. Neste caso, para construir a sentença, a escola deve escolher a Ação, indicar a Quantidade e definir o Objeto da ação. Depois, é necessário descrever o período em que a escola pretende realizar aquela ação e, no campo “Detalhamento ação”, descrever algumas características da atividade que será realizada.</p> 
		<p>Depois de definir a ação e seus objetivos, é necessário responder se são necessários recursos financeiros para realiza-la. Em caso afirmativo, selecione “Sim”. O sistema exibirá o botão “Inserir”. Neste caso, o GT deve indicar a “Categoria da despesa”, escolher um “Item” daquela categoria, definir a “Unidade de referência”, escolher “Quantidade” daquele item, descrever o “Valor unitário”, informar a “Fonte” e escolher se o item será adquirido com recursos da 1ª parcela ou da 2ª parcela. Observe que o saldo de recursos de cada parcela e a natureza da despesa vão diminuindo à medida em que forem inseridos bens ou serviços.</p>
		<p>O sistema calculará o valor total e exibirá na rubrica capital ou custeio, de acordo com a classificação indicada na Portaria 448/2002. Caso exista divergência de classificação da natureza da despesa em relação aos critérios da sua secretaria, não inclua aquele item, a fim de que não haja problemas durante a execução do plano.</p>
		<p>E lembre-se! Antes de inserir as ações, observe o saldo de recursos do Ano 1 e Ano 2 e os respectivos valores de capital e custeio.</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Ação</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Saldo do PDE Escola</td>
			<td id="td_saldoPdeEscolaAcao"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ação</td>
			<td><? $db->monta_combo('aapid', $sql_acao, 'S', 'Selecione', '', '', '', '', 'S', 'aapid','', $aapid); ?> Quantidade: <? echo campo_texto('paaacaoqtd', "S", "S", "Quantidade", 7, 6, "######", "", '', '', 0, 'id="paaacaoqtd"', '', $paaacaoqtd ); ?> Objeto <? $db->monta_combo('oapid', $sql_objeto, 'S', 'Selecione', '', '', '', '', 'S', 'oapid', '', $oapid); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período</td>
			<td><? echo campo_data2('paaperiodoinicio','S', 'S', 'Período inicial', 'S', '', '', $paaperiodoinicio ); ?> a <? echo campo_data2('paaperiodofim','S', 'S', 'Período final', 'S', '', '', $paaperiodofim); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Detalhamento da ação</td>
			<td><? echo campo_textarea( 'paadetalhamento', 'S', 'S', '', '70', '4', '200', '', '', '','','',$paadetalhamento); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Precisa de recursos financeiros?</td>
			<td><input type="radio" onclick="exibePrecisaRecursos(true)" name="paarecurso" value="TRUE" <?=(($paarecurso=="t")?"checked":"") ?> > Sim <input type="radio" name="paarecurso" onclick="exibePrecisaRecursos(false)" value="FALSE" <?=(($paarecurso=="f")?"checked":"") ?> > Não</td>
		</tr>
		<tr id="tr_bens_servicos" style="display:<?php echo $paarecurso && $paarecurso == "t" ? "" : "none" ?>" >
			<td class="SubTituloDireita">Bens e serviços</td>
			<td>
				Clique em "Inserir" para incluir as aquisições e contratações desejadas.
				<p><input type="button" name="inserirbensservicos" value="Inserir" onclick="inserirBens()"></p>
				<div id="div_bensservicos"></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" name="btn_salvar" onclick="submeterAcao();"> <input type="button"  name="btn_cancelar" value="Cancelar" onclick="window.close();"></td>
		</tr>
		
	</table>
	</form>
	</body>
	</html>
	<?
}

function carregarSaldoPdeEscolaAcao($dados) {
	global $db;
	
	$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE codinep='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND cccstatus='A'";
	$cargacapitalcusteio = $db->pegaLinha($sql);
	

	$sql = "SELECT SUM(pabvalorcapital) as pabvalorcapital, SUM(pabvalorcusteiro) as pabvalorcusteiro, pabparcela 
			FROM pdeinterativo.planoacaobemservico pab 
			INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paaid = pab.paaid
			INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.paeid = paa.paeid 
			INNER JOIN pdeinterativo.planoacaoproblema pap ON pap.papid = pae.papid 
			WHERE pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A' 
			GROUP BY pabparcela";
	$planoacaobemservicos = $db->carregar($sql);
	if($planoacaobemservicos[0]) {
		foreach($planoacaobemservicos as $pbs) {
			$gastopbs[$pbs['pabparcela']] = array('pabvalorcapital'=>$pbs['pabvalorcapital'],'pabvalorcusteiro'=>$pbs['pabvalorcusteiro']);
		}
	}
			
	if($cargacapitalcusteio):
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
	<tr>
		<td class="SubTituloDireita">1ª Parcela:</td>
		<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalprimeira']+$cargacapitalcusteio['ccccusteioprimeira']-$gastopbs['P']['pabvalorcapital']-$gastopbs['P']['pabvalorcusteiro']),2,",",".")." ( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalprimeira']-$gastopbs['P']['pabvalorcapital'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteioprimeira']-$gastopbs['P']['pabvalorcusteiro'],2,",",".").")" ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">2ª Parcela:</td>
		<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalsegunda']+$cargacapitalcusteio['ccccusteiosegunda']-$gastopbs['S']['pabvalorcapital']-$gastopbs['S']['pabvalorcusteiro']),2,",",".")." ( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalsegunda']-$gastopbs['S']['pabvalorcapital'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteiosegunda']-$gastopbs['S']['pabvalorcusteiro'],2,",",".").")" ?></td>
	</tr>
	</table>
	<? else : ?>
	<p>Não foram encontrados dados sobre o saldo</p>
	<? endif;
}

function salvaAcaoAjax($dados)
{
	global $db;
	extract($dados);
	$paeid = !$paeid ? "null" : $paeid;
	$aapid = !$aapid ? "null" : $aapid;
	$oapid = !$oapid ? "null" : $oapid;
	$paaacaoqtd = !$paaacaoqtd ? "null" : $paaacaoqtd;
	$paaperiodoinicio = !$paaperiodoinicio || strlen($paaperiodoinicio)!=10 ? "null" : "'".formata_data_sql($paaperiodoinicio)."'";
	$paaperiodofim = !$paaperiodofim || strlen($paaperiodofim)!=10 ? "null" : "'".formata_data_sql($paaperiodofim)."'";
	$paadetalhamento = !$paadetalhamento ? "null" : "'".$paadetalhamento."'";
	$paarecurso = !$paarecurso ? "null" : "'".$paarecurso."'";
	$paacustototal = !$paacustototal ? "null" : "'".$paacustototal."'";
	$paastatus = "'A'";
	
	$sql = "INSERT INTO 
				pdeinterativo.planoacaoacao
			(paeid,aapid,oapid,paaacaoqtd,paaperiodoinicio,paaperiodofim,paadetalhamento,paarecurso,paacustototal,paastatus)
				VALUES
			($paeid,$aapid,$oapid,$paaacaoqtd,$paaperiodoinicio,$paaperiodofim,$paadetalhamento,$paarecurso,$paacustototal,$paastatus)
				RETURNING paaid";
	$paaid = $db->pegaUm($sql);
	$db->commit();
	apagarCachePdeInterativo();
	ob_clean();
	echo $paaid;
	
}

function atualizarAcaoPlanoEstrategico($dados) {
	global $db;
	if(is_numeric($dados['paaid'])) {
		
		if(formata_data_sql($dados['paaperiodoinicio'])>formata_data_sql($dados['paaperiodofim'])) die("<script>alert('Data Inicial maior do Data Final');window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid=".$dados['paeid']."&paaid=".$dados['paaid']."';</script>");
		
		$sql = "UPDATE pdeinterativo.planoacaoacao
	   			SET aapid='".$dados['aapid']."', oapid='".$dados['oapid']."', 
	   				paaacaoqtd='".$dados['paaacaoqtd']."', paaperiodoinicio='".formata_data_sql($dados['paaperiodoinicio'])."', 
	    			paaperiodofim='".formata_data_sql($dados['paaperiodofim'])."', 
	       			paadetalhamento='".$dados['paadetalhamento']."', paarecurso=".(($dados['paarecurso'])?$dados['paarecurso']:"TRUE")."  
	 			WHERE paaid='".$dados['paaid']."';";
		$db->executar($sql);
		$db->commit();
		
		apagarCachePdeInterativo();
				
		echo "<script>
				window.opener.carregarPlanoEstrategicoDimensao();
				alert('Ação atualizada com sucesso');
				window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid=".$dados['paeid']."&paaid=".$dados['paaid']."';
			  </script>";
	} else {
		echo "<script>
				alert('Não foi possível atualizar ação. Feche a tela e tente novamente');
				window.close();
			  </script>";
	}
	
}

function comboItensCategorias($dados) {
	global $db;
	$sql = "SELECT ciaid as codigo, ciadesc as descricao FROM pdeinterativo.categoriaitemacao WHERE ciastatus='A' AND cacid='".$dados['cacid']."'";
	$db->monta_combo('ciaid', $sql, 'S', 'Selecione', 'selecionarItem', '', '', '', 'S', 'ciaid','', $dados['ciaid']);
}

function pegaUnidadeReferenciaItem($dados) {
	global $db;
	$sql = "SELECT ure.uredesc FROM pdeinterativo.unidadereferencia ure 
			INNER JOIN pdeinterativo.categoriaitemacao cia ON cia.ureid = ure.ureid 
			WHERE ciaid='".$dados['ciaid']."'";
	
	$unrefe = $db->pegaUm($sql);
	
	echo (($unrefe)?$unrefe:"Não cadastrado");
	
}



function atualizarBensServicos($dados) {
	global $db;
	// verificando se existe saldo 
	$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE codinep='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND cccstatus='A'";
	$cargacapitalcusteio = $db->pegaLinha($sql);
	
	$problema = false;
	if($cargacapitalcusteio) {
		if($dados['pabparcela']=="P") $alt="primeira";
		if($dados['pabparcela']=="S") $alt="segunda";
		
		$outros_pabs = $db->pegaLinha("SELECT COALESCE(SUM(pabvalorcapital),0) as pabvalorcapital, COALESCE(SUM(pabvalorcusteiro),0) as pabvalorcusteiro 
								  	   FROM pdeinterativo.planoacaobemservico p 
								  	   INNER JOIN pdeinterativo.planoacaoacao po ON po.paaid = p.paaid 
								  	   INNER JOIN pdeinterativo.planoacaoestrategia pe ON pe.paeid = po.paeid 
								  	   INNER JOIN pdeinterativo.planoacaoproblema pp ON pp.papid = pe.papid  
								  	   WHERE pabparcela='".$dados['pabparcela']."' AND 
	   										 pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
								  	   		 pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A' AND pabid!='".$dados['pabid']."'");
		
		if((str_replace(array(".",","),array("","."),$dados['pabvalorcapital'])+$outros_pabs['pabvalorcapital']) > $cargacapitalcusteio['ccccapital'.$alt]) $problema=true;
		if((str_replace(array(".",","),array("","."),$dados['pabvalorcusteiro'])+$outros_pabs['pabvalorcusteiro']) > $cargacapitalcusteio['ccccusteio'.$alt]) $problema=true;
		
		if($problema) die("Não há saldo disponível nesta categoria de despesa ou nesta parcela.");
		
	}
	// FIM - verificando se existe saldo
	
	$sql = "UPDATE pdeinterativo.planoacaobemservico
   			SET ciaid='".$dados['ciaid']."', 
   				pabqtd='".$dados['pabqtd']."', pabvalor='".str_replace(array(".",","),array("","."),$dados['pabvalor'])."', 
   				pabvalorcapital=".(($dados['pabvalorcapital'])?"'".str_replace(array(".",","),array("","."),$dados['pabvalorcapital'])."'":"NULL").", 
       			pabvalorcusteiro=".(($dados['pabvalorcusteiro'])?"'".str_replace(array(".",","),array("","."),$dados['pabvalorcusteiro'])."'":"NULL").", 
       			pabfonte='".$dados['pabfonte']."', pabparcela=".(($dados['pabparcela'])?"'".$dados['pabparcela']."'":"NULL")."  
 			WHERE pabid='".$dados['pabid']."';";
	
	$db->executar($sql);
	$db->commit();
	$sql = "UPDATE pdeinterativo.planoacaoacao SET paacustototal=(SELECT SUM(COALESCE(pabvalorcapital,0)+COALESCE(pabvalorcusteiro,0)) 
			FROM pdeinterativo.planoacaobemservico WHERE pabstatus='A' AND paaid IN(SELECT paaid FROM pdeinterativo.planoacaobemservico WHERE pabid='".$dados['pabid']."' AND pabstatus='A')) 
			WHERE paaid IN(SELECT paaid FROM pdeinterativo.planoacaobemservico WHERE pabid='".$dados['pabid']."' AND pabstatus='A')";
	
	$db->executar($sql);
	$db->commit();
	
	apagarCachePdeInterativo();
	
}

function gerenciarBensServicos($dados) {
	global $db;

	$sql_categoria = "SELECT cacid as codigo, cacdesc as descricao, cacnatureza FROM pdeinterativo.categoriaacao WHERE cacstatus='A'";
	
	if($_SESSION['pdeinterativo_vars']['pditempdeescola']=="t") {
		$sql_fonte = array(0 => array("codigo" => "P", "descricao" => "PDDE/PDE Escola"),
						   1 => array("codigo" => "O", "descricao" => "Outras"));
	} else {
		$sql_fonte = array(0 => array("codigo" => "P", "descricao" => "Fonte da Secretaria"),
						   1 => array("codigo" => "O", "descricao" => "Outras"));
	}
	
	if($dados['pabid']) {
		$sql = "SELECT pab.pabid, cia.cacid, pab.ciaid as ciaid, pab.ureid as ureid, pabqtd, trim(to_char(pabvalor,'999g999g999d99')) as pabvalor,
  					   trim(to_char(pabvalorcapital,'999g999g999d99')) as pabvalorcapital,
  					   trim(to_char(pabvalorcusteiro,'999g999g999d99')) as pabvalorcusteiro,
					   pabfonte,
  					   pabparcela  
  				FROM pdeinterativo.planoacaobemservico pab 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				WHERE pab.pabid='".$dados['pabid']."' AND pab.pabstatus='A'";
		$planoacaobemservico = $db->pegaLinha($sql);
		
		if($planoacaobemservico) {
			extract($planoacaobemservico);
			$requisicao = "atualizarBensServicos";
		} else {
			$requisicao = "inserirBensServicos";
		}

	} else {
		$requisicao = "inserirBensServicos";
	}
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script>
	function selecionarCategoriaItem(cacid) {
		if(cacid!='') {
			document.getElementById('pabvalorcapital').value = '';
			document.getElementById('pabvalorcusteiro').value = '';
			document.getElementById('td_itemcategoria').innerHTML = 'Carregando...';
			somarBensServicos('');
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=comboItensCategorias<?=(($ciaid)?"&ciaid=".$ciaid:"") ?>&cacid="+cacid,
		   		async: false,
		   		success: function(msg){document.getElementById('td_itemcategoria').innerHTML = msg;}
		 		});
		} else {
			document.getElementById('td_itemcategoria').innerHTML = 'Selecione categoria';
		}
	}
	
	function selecionarItem(ciaid) {
		if(ciaid) {
			document.getElementById('td_unreferencia').innerHTML = 'Carregando...';
			somarBensServicos('');
			jQuery.ajax({
		   		type: "POST",
		   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
		   		data: "requisicao=pegaUnidadeReferenciaItem&ciaid="+ciaid,
		   		async: false,
		   		success: function(msg){document.getElementById('td_unreferencia').innerHTML = msg;}
		 		});
	 	} else {
	 		document.getElementById('td_unreferencia').innerHTML = 'Selecione item';
	 	}
	}

	
	function submeterBensServicos() {
		if(document.getElementById('cacid').value=='') {
			alert('Selecione uma categoria');
			return false;
		}
		if(document.getElementById('ciaid').value=='') {
			alert('Selecione um item');
			return false;
		}
		document.getElementById('pabqtd').value = mascaraglobal('#####',document.getElementById('pabqtd').value);
		if(document.getElementById('pabqtd').value=='') {
			alert('Preencha a quantidade');
			return false;
		}
		document.getElementById('pabvalor').value = mascaraglobal('###.###.###,##',document.getElementById('pabvalor').value);
		if(document.getElementById('pabvalor').value=='') {
			alert('Preencha o valor unitário');
			return false;
		}
		if(document.getElementById('pabfonte').value=='') {
			alert('Selecione a fonte');
			return false;
		}
		if(document.getElementById('pabfonte').value!='O') {
			if(jQuery("[name^='pabparcela']:checked").length == 0) {
				alert('Selecione se precisa de recurso');
				return false;
			}
		}
		
		document.getElementById('btnsalvar').disabled=true;
		document.getElementById('btncancelar').disabled=true;
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
	   		data: "requisicao=<?=$requisicao ?>&"+jQuery('#formulario2').serialize(),
	   		async: false,
	   		success: function(msg){
	   		if(msg) {
	   			alert(msg);
				document.getElementById('btnsalvar').disabled=false;
				document.getElementById('btncancelar').disabled=false;
	   		} else {
			 	carregarBensServicos();
			 	carregarSaldoPdeEscolaAcao();
			 	closeMessage();
		 	}
	   		}
	 		});
	 		

		
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
	
	function exibeParcela(fonte) {
		if(fonte=="O") {
			document.getElementById('tr_parcela').style.display='none';
		} else {
			document.getElementById('tr_parcela').style.display='';
		}
	}

	
	function somarBensServicos(obj) {
	
		var bensservicos = new Array();
		
		if(document.getElementById('cacid').value=='') {
			alert('Selecione categoria');
			obj.value='';
			return false;
		}
		<? 
		$bensServicos = $db->carregar($sql_categoria);
		if($bensServicos[0]) {
			foreach($bensServicos as $bs) {
				echo "bensservicos[".$bs['codigo']."]='".$bs['cacnatureza']."';";
			}
		}
		?>
		var valorunit = replaceAll(replaceAll(document.getElementById('pabvalor').value, '.', ''),',','.');
		if(document.getElementById('pabqtd').value && valorunit) {
			var valortotal = parseFloat(document.getElementById('pabqtd').value)*parseFloat(valorunit);
			if(bensservicos[document.getElementById('cacid').value]=='A') {
				document.getElementById('pabvalorcapital').value = mascaraglobal('###.###.###,##',valortotal.toFixed(2));
			}
			if(bensservicos[document.getElementById('cacid').value]=='U') {
				document.getElementById('pabvalorcusteiro').value = mascaraglobal('###.###.###,##',valortotal.toFixed(2));
			}
		}
	}
	
	jQuery(document).ready(function() {
	<? if($cacid): ?>
	selecionarCategoriaItem('<?=$cacid ?>');
	<? endif; ?>
	});
	
	</script>
	<form method="post" id="formulario2">
	<input type="hidden" name="paaid" value="<?=$dados['paaid'] ?>">
	<input type="hidden" name="pabid" value="<?=$pabid ?>">
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Bens e Serviços</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Categoria</td>
			<td><? $db->monta_combo('cacid', $sql_categoria, 'S', 'Selecione', 'selecionarCategoriaItem', '', '', '', 'S', 'cacid','', $cacid); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Item</td>
			<td id="td_itemcategoria">Selecione categoria</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade de referência</td>
			<td id="td_unreferencia">Selecione item</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade</td>
			<td><? echo campo_texto('pabqtd', 'S', 'S', 'Quantidade', 6, 5, "#####", "", '', '', 0, 'id="pabqtd"', 'somarBensServicos(this);', $pabqtd ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor Unitário(R$)</td>
			<td><? echo campo_texto('pabvalor', 'S', 'S', 'Valor Unitário(R$)', 16, 16, "###.###.###,##", "", '', '', 0, 'id="pabvalor"', 'somarBensServicos(this);', $pabvalor ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Capital(R$)</td>
			<td><? echo campo_texto('pabvalorcapital', 'S', 'N', 'Capital(R$)', 16, 16, "###.###.###,##", "", '', '', 0, 'id="pabvalorcapital"', '', $pabvalorcapital ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custeio(R$)</td>
			<td><? echo campo_texto('pabvalorcusteiro', 'S', 'N', 'Custeio(R$)', 16, 16, "###.###.###,##", "", '', '', 0, 'id="pabvalorcusteiro"', '', $pabvalorcusteiro ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Fonte</td>
			<td><? $db->monta_combo('pabfonte', $sql_fonte, 'S', 'Selecione', 'exibeParcela', '', '', '', 'S', 'pabfonte','', $pabfonte); ?></td>
		</tr>
		<tr id="tr_parcela" style="display:none;">
			<td class="SubTituloDireita">Parcela / Ano</td>
			<td>
			<input type="radio" name="pabparcela" value="P" <?=(($pabparcela=="P")?"checked":"") ?>>1ª Parcela (2011)<br>
			<input type="radio" name="pabparcela" value="S" <?=(($pabparcela=="S")?"checked":"") ?>>2ª Parcela (2012)
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" id="btnsalvar" value="Salvar" onclick="submeterBensServicos();"> <input id="btncancelar" type="button" value="Cancelar" onclick="closeMessage();"></td>
		</tr>
	</table>
	</form>
	<?

}

function inserirBensServicos($dados) {
	global $db;
	
	// verificando se existe saldo 
	$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE codinep='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND cccstatus='A'";
	$cargacapitalcusteio = $db->pegaLinha($sql);
	
	$problema = false;
	
	if(!is_numeric($dados['paaid']) || !$dados['paaid']) die("Houve problemas na inserção do registro. Feche esta tela e reinicie o procedimento.");

	if($cargacapitalcusteio && $dados['pabfonte']!="O") {
		if($dados['pabparcela']=="P") $alt="primeira";
		if($dados['pabparcela']=="S") $alt="segunda";
		
		$outros_pabs = $db->pegaLinha("SELECT COALESCE(SUM(pabvalorcapital),0) as pabvalorcapital, COALESCE(SUM(pabvalorcusteiro),0) as pabvalorcusteiro 
								  	   FROM pdeinterativo.planoacaobemservico p 
								  	   INNER JOIN pdeinterativo.planoacaoacao po ON po.paaid = p.paaid 
								  	   INNER JOIN pdeinterativo.planoacaoestrategia pe ON pe.paeid = po.paeid 
								  	   INNER JOIN pdeinterativo.planoacaoproblema pp ON pp.papid = pe.papid  
								  	   WHERE pabparcela='".$dados['pabparcela']."' AND 
	   										 pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
								  	   		 pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A'");
		
		if((str_replace(array(".",","),array("","."),$dados['pabvalorcapital'])+$outros_pabs['pabvalorcapital']) > $cargacapitalcusteio['ccccapital'.$alt]) $problema=true;
		if((str_replace(array(".",","),array("","."),$dados['pabvalorcusteiro'])+$outros_pabs['pabvalorcusteiro']) > $cargacapitalcusteio['ccccusteio'.$alt]) $problema=true;
		
		if($problema) die("Não há saldo disponível nesta categoria de despesa ou nesta parcela.");
		
	}
	// FIM - verificando se existe saldo

	
	$sql = "INSERT INTO pdeinterativo.planoacaobemservico(
            paaid, ciaid, pabqtd, pabvalor, pabvalorcapital, 
            pabvalorcusteiro, pabfonte, pabparcela, pabstatus)
    		VALUES ('".$dados['paaid']."', '".$dados['ciaid']."',  
    				'".$dados['pabqtd']."', '".str_replace(array(".",","),array("","."),$dados['pabvalor'])."', 
    				".(($dados['pabvalorcapital'])?"'".str_replace(array(".",","),array("","."),$dados['pabvalorcapital'])."'":"NULL").", 
    				".(($dados['pabvalorcusteiro'])?"'".str_replace(array(".",","),array("","."),$dados['pabvalorcusteiro'])."'":"NULL").",
    				'".$dados['pabfonte']."', ".(($dados['pabparcela'])?"'".$dados['pabparcela']."'":"NULL").", 'A');";
	
	$db->executar($sql);
	$db->commit();
		
	$sql = "UPDATE pdeinterativo.planoacaoacao SET paacustototal=(SELECT SUM(COALESCE(pabvalorcapital,0)+COALESCE(pabvalorcusteiro,0)) FROM pdeinterativo.planoacaobemservico WHERE pabstatus='A' AND paaid='".$dados['paaid']."') 
			WHERE paaid='".$dados['paaid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	apagarCachePdeInterativo();
	
	
}

function carregarBensServicos($dados) {
	global $db;
	if($dados['paaid']) {
		
		if(!is_numeric($dados['paaid'])) die("Ocorreu erro durante o carregamento. Feche a tela e tente novamente.");
		
		$sql = "SELECT '<center><span style=\"white-space:nowrap;\" ><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"displayMessage(\'pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarBensServicos&pabid='||pab.pabid||'\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"deletarBensServicos('||pab.pabid||')\"></span></center>' as acao, cac.cacdesc, cit.ciadesc, unr.uredesc, pab.pabqtd,
					   pabvalor,
  					   pabvalorcapital,
  					   pabvalorcusteiro,
  					   CASE WHEN pabfonte='P' THEN 'Fonte da secretaria' ELSE 'Outras' END,
  					   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' 
  					   		WHEN pabparcela='S' THEN '2ª Parcela (2012)' 
  					   		ELSE '&nbsp;' END as pabfonte
  					     
				FROM pdeinterativo.planoacaobemservico pab 
				LEFT JOIN pdeinterativo.categoriaitemacao cit ON cit.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cit.cacid
				LEFT JOIN pdeinterativo.unidadereferencia unr ON unr.ureid = cit.ureid
				WHERE paaid='".$dados['paaid']."' AND pabstatus='A'";
	} else {
		$sql = array();
	}
	$cabecalho = array("Ação","Categoria","Item","Unidade de referencia","Qtd","Valor(R$)","Capital(R$)","Custeio(R$)","Fonte","Parcela/Ano");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
}


function verificaBensServicos($dados)
{
	global $db;
	
	if(!is_numeric($dados['paaid'])) {
		return false;
	}
	
	if($dados['paaid']) {
		$sql = "SELECT count(*)
				FROM pdeinterativo.planoacaobemservico pab
				WHERE paaid='".$dados['paaid']."' AND pabstatus='A'";
		$count = $db->pegaUm($sql);
		if($count){
			echo $count;
		}else{
			return false;
		}
	} else {
		return false;
	}
}

function excluirBensServicosPorAcao($dados)
{
	global $db;
	if(is_numeric($dados['paaid'])) {
		$sql = "UPDATE pdeinterativo.planoacaobemservico set pabstatus = 'I'
				WHERE paaid='".$dados['paaid']."' AND pabstatus='A'";
		$db->executar($sql);
		$db->commit();
		apagarCachePdeInterativo();
		
		return true;
	} else {
		return false;
	}
}

function inserirAcaoPlanoEstrategico($dados) {
	global $db;
	
	if(formata_data_sql($dados['paaperiodoinicio'])>formata_data_sql($dados['paaperiodofim'])) die("<script>alert('Data Inicial maior do Data Final');window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid=".$dados['paeid']."&paaid=';</script>");
	if(!$dados['paarecurso']) $dados['paarecurso']="TRUE";
	
	$sql = "INSERT INTO pdeinterativo.planoacaoacao(
            paeid, aapid, oapid, paaacaoqtd, paaperiodoinicio, paaperiodofim, 
            paadetalhamento, paarecurso, paacustototal, paastatus)
    		VALUES ('".$dados['paeid']."', 
    				'".$dados['aapid']."', 
    				'".$dados['oapid']."', 
    				'".$dados['paaacaoqtd']."', 
    				".(($dados['paaperiodoinicio'])?"'".formata_data_sql($dados['paaperiodoinicio'])."'":"NULL").", 
    				".(($dados['paaperiodofim'])?"'".formata_data_sql($dados['paaperiodofim'])."'":"NULL").", 
    				'".$dados['paadetalhamento']."', 
    				".$dados['paarecurso'].",
            		0, 
            		'A') RETURNING paaid;";
	$paaid = $db->pegaUm($sql);
	$db->commit();
	
	apagarCachePdeInterativo();
	
	echo "<script>
			window.opener.carregarPlanoEstrategicoDimensao();
			alert('Ação inserida com sucesso');
			window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid=".$dados['paeid']."&paaid=".$paaid."';
		  </script>";
}

function planoestrategicoDimensao($dados) {
	global $db;
	$sql_objetivo = "SELECT aoaid as codigo, aoadesc as descricao FROM pdeinterativo.apoioobjetivoplanoacao WHERE aoastatus='A' and abaid = {$dados['abaid']}";
	
	$aoaid = $db->pegaUm("SELECT aoaid 
						  FROM pdeinterativo.objetivoplanoacao 
						  WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND abaid='".$dados['abaid']."' AND opastatus='A'");

	?>
	<tr>
		<td style="cursor:pointer" onclick="exibeDimensao('<?php echo $dados['abaid'] ?>')" class="SubTituloTabela center bold" colspan="2"><?=$dados['abadescricao'] ?></td>
	</tr>

	<?php

		$funcao = "listametas_".$dados['abacod'];
		$metas = $funcao($dados);
	?>
	<tr class="dimensao_<?php echo $dados['abaid'] ?>" >
		<td colspan="2" valign="top">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<?
		$sql = "SELECT prb.papid as codigo, prb.papdescricao as descricao 
				FROM pdeinterativo.planoacaoproblema prb 
				WHERE prb.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
					  prb.papstatus='A' AND 
					  prb.abacod='".$dados['abacod']."'   
				ORDER BY prb.papid";
		$problemas = $db->carregar($sql);
		?>
			<?php if($problemas[0]): ?>
			
				<?php if($metas): ?>
					<tr class="dimensao_<?php echo $dados['abaid'] ?>" >
						<td class="SubTituloDireita">Metas</td>
						<td colspan="2"><?php echo implode("<br>",$metas); ?></td>
					</tr>
					<tr class="dimensao_<?php echo $dados['abaid'] ?>" >
						<td class="SubTituloDireita">Objetivo</td>
						<td colspan="2"><? $db->monta_combo('aoaid['.$dados['abaid'].']', $sql_objetivo, 'S', 'Selecione', 'salvarFormularioParcialmente', '', '', '', 'N', 'aoaid['.$dados['abaid'].']', '',$aoaid); ?></td>
					</tr>
				<?php else: ?>
					<tr class="dimensao_<?php echo $dados['abaid'] ?>" >
						<td colspan="3" class="center">Não foram identificados metas</td>
					</tr>
				<?php endif; ?>

				<tr>
					<td class="SubTituloCentro" style="border: 1px solid #000;" width="33%">Problemas</td>
					<td class="SubTituloCentro" style="border: 1px solid #000;" width="25%">Estratégia</td>
					<td class="SubTituloCentro" style="border: 1px solid #000;" width="42%">Ação</td>
				</tr>
			<?php endif; ?>
			<?
			$sql_responsaveis = "SELECT DISTINCT * FROM (
								 SELECT 'P_'||p.pesid as codigo, p.pesnome as descricao FROM pdeinterativo.pessoa p 
								 INNER JOIN pdeinterativo.pessoagruptrab pa ON pa.pesid = p.pesid 
								 INNER JOIN pdeinterativo.grupotrabalho gt ON gt.grtid = pa.grtid 
								 WHERE gt.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' 
								 UNION ALL 
								 SELECT 'P_'||p.pesid as codigo, p.pesnome as descricao FROM pdeinterativo.pessoa p 
								 INNER JOIN pdeinterativo.membroconselho mc ON mc.pesid = p.pesid 
								 WHERE mc.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' 
								 UNION ALL 
								 SELECT 'P_'||p.pesid as codigo, p.pesnome as descricao FROM pdeinterativo.pessoa p 
								 INNER JOIN pdeinterativo.demaisprofissionais dp ON dp.pesid = p.pesid 
								 WHERE dp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'
								 UNION ALL 
								 SELECT 'D_'||d.pk_cod_docente as codigo, d.no_docente as descricao FROM educacenso_2010.tab_docente_disc_turma t 
								 INNER JOIN educacenso_2010.tab_docente d ON d.pk_cod_docente = t.fk_cod_docente 
								 INNER JOIN pdeinterativo.respostadocente r ON r.pk_cod_docente = d.pk_cod_docente
								 WHERE fk_cod_entidade='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND r.rdovinculo IN('T','E')
								 ) foo
								 ";
			
			if($problemas):
			$i=1;
			foreach($problemas as $key => $problema) :
			
			if($problema['codigo']) {
				$sql = "SELECT * FROM pdeinterativo.planoacaoestrategia pe 
						INNER JOIN pdeinterativo.estrategiaplanoacaoapoio ea ON ea.epaid = pe.epaid 
						WHERE pe.papid='".$problema['codigo']."' AND paestatus='A'";
				$estrategias = $db->carregar($sql);
			}
			
			?>
			<tr>
				<td rowspan="<?=(count($estrategias)+1) ?>" valign="top" class="bordapreto"><?=$problema['descricao'] ?>&nbsp;</td>
				<td colspan="2" class="bordapreto"><input type="button" value="Inserir estratégia" name="estrategia" onclick="gerenciarEstrategias('<?=$dados['abaid'] ?>','<?=$problema['codigo'] ?>');"></td>
			</tr>
			<?
			if($estrategias[0]) :
				foreach($estrategias as $estrategia):
				
			?>
			<tr>
				<td class="bordapreto"><?=$estrategia['epadesc'] ?><br>
				<br>
				<b>Responsavel:</b><br>
				<? $db->monta_combo('pesid_estrategias['.$estrategia['paeid'].']', $sql_responsaveis, 'S', 'Selecione', 'salvarFormularioParcialmente', '', '', '200', 'N', 'pesid_estrategias', '', $estrategia['respid']); ?>
				</td>
				<td valign="top" class="bordapreto">
				<p><input type="button" value="Inserir ação" onclick="gerenciarAcao('<?=$estrategia['paeid'] ?>','');"></p>
				<? 
				$arrAcao = false;
				$sql = "SELECT paaid,'<span style=\"white-space:nowrap;\" ><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"gerenciarAcao('||paa.paeid||','||paa.paaid||');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAcao('||paa.paaid||');\"></span>' as acao, aap.aapdesc, paa.paaacaoqtd, oap.oapdesc, paa.paadetalhamento, to_char(paaperiodoinicio,'dd/mm/YYYY')||' a '||to_char(paaperiodofim,'dd/mm/YYYY') as periodo, 
						CASE WHEN paarecurso=TRUE THEN 'Sim' ELSE 'Não' END as recurso, paa.paacustototal 
						FROM pdeinterativo.planoacaoacao paa 
						LEFT JOIN pdeinterativo.acaoapoio aap ON aap.aapid = paa.aapid 
						LEFT JOIN pdeinterativo.objetoapoio oap ON oap.oapid = paa.oapid
						WHERE paeid='".$estrategia['paeid']."' AND paa.paastatus='A'";
				
				$arrDados = $db->carregar($sql);
				if($arrDados){
					$n = 0;
					foreach($arrDados as $dadosCia){
						$arrAcao[$n] = $dadosCia;
						$sql = "select 
									cia.ciadesc
								from 
									pdeinterativo.planoacaobemservico ben
								inner join
									pdeinterativo.categoriaitemacao cia ON cia.ciaid = ben.ciaid 
								where 
									paaid = {$dadosCia['paaid']}";
						$arrServicos = $db->carregarColuna($sql);
						$arrServicos = !$arrServicos ? array("N/A") : $arrServicos;
						$arrAcao[$n]['servicos'] = implode(", ",$arrServicos);
						unset($arrAcao[$n]['paaid']);
						$n++;
					}
				}
				$arrAcao = !$arrAcao ? array() : $arrAcao;
				$cabecalho = array("&nbsp","Ação","Qtd","Objeto","Detalhamento","Período","Recurso?","Total ação","Bens e serviços");
				$db->monta_lista_simples($arrAcao,$cabecalho,50,5,'N','100%',$par2); 
				?>
				</td>
			</tr>
			<? endforeach; ?>
			<? else: ?>
			<tr>
				<td class="SubTituloCentro" style="border: 1px solid #000;" colspan="2">Não existem estratégias cadastradas</td>
			</tr>
			<? endif; ?>

			<? endforeach; ?>
			<? endif; ?>
		</table>		
		</td>
	</tr>
	<?
}


function listaproblemas_diagnostico_2_distorcaoeaproveitamento($dados) {
	global $db;
	
	
	//Início - Distorção Idade-Série (D,D)
	?>
	<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(false,"M",null); ?>
	<?php $arrRespEscola = array() ?>
	<?php if( $arrDistorcao && verificaTurmasCNE(array_unique($arrDistorcao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcao)." turma(s) com nº de matrículas superior ao parâmetro do CNE." ?>
			<?php $arrProblemas['codigo'][] = "matricula_".implode("_",$arrDistorcao) ?>
	<?php endif; ?>
	
	<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(null,"D","D"); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","D",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if( $arrDistorcao && verificaCheckBoxTaxa("distorcao",array_unique($arrDistorcao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcao)." turma(s) com taxa de distorção superior à média do Brasil." ?>
			<?php $arrProblemas['codigo'][] = "distorcao_".implode("_",$arrDistorcao) ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['codigo'][] = $resp['repid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php $arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R"); ?>
	<?php $arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A"); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","A",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if( $arrDistorcaoReprovacao && verificaCheckBoxTaxa("reprovacao",array_unique($arrDistorcaoReprovacao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcaoReprovacao)." turma(s) com taxa de reprovação superior à média do Brasil." ?>
			<?php $arrProblemas['codigo'][] = "reprovacao_".implode("_",$arrDistorcaoReprovacao) ?>
	<?php endif; ?>
	<?php if( $arrDistorcaoAbandono && verificaCheckBoxTaxa("abandono",array_unique($arrDistorcaoAbandono)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcaoAbandono)." turma(s) com taxa de abandono superior à média do Brasil." ?>
			<?php $arrProblemas['codigo'][] = "abandono_".implode("_",$arrDistorcaoAbandono) ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['codigo'][] = $resp['repid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php $arrTurmas = recuperaTurmasCriticasPorEscola();?>
	<?php $arrTaxaReprovacaoBrasil = carregaTaxa(); ?>
	<?php $arrDisciplinas = retornaDisciplinasTurma(); ?>
	<?php $arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina(); ?>
	<?php $arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar(); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","C",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if($arrTurmas): ?>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<?php foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
				<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
					<?php foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
						<?php if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ): ?>
							<?php if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['U']): ?>
								<?php $arrTurmasCriticasReprovacao["U"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'] ?>
								<?php $arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'] ?>
								<?php $arrTurmasC["U"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																				"disciplina" => $ds['no_disciplina'],
																				"serie" => $em['serie'],
																				"turma" => $em['turma'],
																				"horario" => $em['hrinicio']." - ".$em['hrfim'],
																				"taxaBrasil" => $arrTaxaReprovacaoBrasil['U']." %",
																				"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																			   ) ?>
							<?php endif; ?>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			<?php endforeach; ?>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<?php foreach($arrTurmas['Ensino Médio'] as $em): ?>
				<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
					<?php foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
						<?php if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ): ?>
							<?php if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['M']): ?>
								<?php $arrTurmasCriticasReprovacao["M"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'] ?>
								<?php $arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'] ?>
								<?php $arrTurmasC["M"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																				"disciplina" => $ds['no_disciplina'],
																				"serie" => $em['serie'],
																				"turma" => $em['turma'],
																				"horario" => $em['hrinicio']." - ".$em['hrfim'],
																				"taxaBrasil" => $arrTaxaReprovacaoBrasil['M']." %",
																				"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																			   ) ?>
							<?php endif; ?>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			<?php endforeach; ?>
		<?php endif; ?>
	<?php endif; ?>
	<?php if($arrTurmasCriticasReprovacao["U"]): ?>
		<?php foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas): ?>
			<?php if( verificaCheckBoxDisciplina($disciplina,$arrTurmas) ): ?>
				<?php $arrProblemas['descricao'][] = count($arrTurmas)." turma(s) do ensino fundamental aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil." ?>
				<?php $arrProblemas['codigo'][] = "disciplina_{$disciplina}_".implode("_",$arrTurmas) ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrTurmasCriticasReprovacao["M"]): ?>
		<?php foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas): ?>
			<?php if( verificaCheckBoxDisciplina($disciplina,$arrTurmas) ): ?>
				<?php $arrProblemas['descricao'][] = count($arrTurmas)." turma(s) do ensino médio aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma(\'m_$disciplina\')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
				<?php $arrProblemas['codigo'][] = "disciplina_{$disciplina}_".implode("_",$arrTurmas) ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['codigo'][] = $resp['repid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif;
	if($arrProblemas['descricao']){
		foreach($arrProblemas['descricao'] as $chave => $prob){
			$arrProblemasDistorcaoAproveitamento[$chave]['descricao'] = $prob;
			$arrProblemasDistorcaoAproveitamento[$chave]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$arrProblemas['codigo'][$chave]."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$arrProblemasDistorcaoAproveitamento[$chave]['codigo']){
				$arrProblemasDistorcaoAproveitamento[$chave]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$arrProblemas['codigo'][$chave]."', 'A', '".$prob."', 'diagnostico_2_distorcaoeaproveitamento') RETURNING papid;");
				$db->commit();
			}
		}	
	}
	//Fim - Distorção Idade-Série (D,D)

	return $arrProblemasDistorcaoAproveitamento;
}

function listametas_diagnostico_2_distorcaoeaproveitamento() {
	 global $db;
	 
	 $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(null,"D","D");
	 
	 $sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumdistorcao > 0
						THEN round(( (dianumdistorcao::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumdistorcao = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
			and
				dia.diasubmodulo = 'D'
			and
				dianumdistorcao is not null
			and
				dia.diastatus = 'A'
			order by
				no_etapa_ensino";
	
	$arrDados = $db->carregar($sql);
	
	if( $arrDados && $arrDistorcao && verificaCheckBoxTaxa("distorcao",array_unique($arrDistorcao)) ){
		foreach($arrDados as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'D' and mt.metdetalhe='Ensino Fundamental' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid
				ORDER BY mt.metid";
		
				$metas_idade_serie_ef = $db->carregar($sql);
				continue;
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'D' and mt.metdetalhe='Ensino Médio' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid
				ORDER BY mt.metid";
		
				$metas_idade_serie_em = $db->carregar($sql);
				continue;
			}
		}
	}
	 
	if($metas_idade_serie_ef[0]) {
		foreach($metas_idade_serie_ef as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	if($metas_idade_serie_em[0]) {
		foreach($metas_idade_serie_em as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	
	$arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R");
	$arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A");
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumreprovado > 0
						THEN round(( (dianumreprovado::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma)::numeric)*100))
					when dianumreprovado = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
			and
				dia.diasubmodulo = 'A'
			and
				dia.diastatus = 'A'
			and
				dia.dianumreprovado is not null
			order by
				no_etapa_ensino";
	
	$arrDadosReprovado = $db->carregar($sql);
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumabandono > 0
						THEN round(( (dianumabandono::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumabandono = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
			and
				dia.diasubmodulo = 'A'
			and
				dia.diastatus = 'A'
			and
				dia.dianumabandono is not null
			order by
				no_etapa_ensino";
	
	$arrDadosAbandono = $db->carregar($sql);
	
	if( $arrDadosAbandono && $arrDistorcaoAbandono && verificaCheckBoxTaxa("abandono",array_unique($arrDistorcaoAbandono)) ){
		foreach($arrDadosAbandono as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'A' and mt.metdetalhe='Ensino Fundamental' and metdesc ilike ('%abandono%') and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid
				ORDER BY mt.metid";
		
				$metas_aproveitamento_escolar_abandono_ef = $db->carregar($sql);
				continue;
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'A' and mt.metdetalhe='Ensino Médio' and metdesc ilike ('%abandono%') and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
				$metas_aproveitamento_escolar_abandono_em = $db->carregar($sql);
				continue;
			}
		}
	}
	
	if( $arrDadosReprovado && $arrDistorcaoReprovacao && verificaCheckBoxTaxa("reprovacao",array_unique($arrDistorcaoReprovacao)) ){
		foreach($arrDadosReprovado as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'A' and mt.metdetalhe='Ensino Fundamental' and metdesc ilike ('%reprovação%') and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
				$metas_aproveitamento_escolar_reprovacao_ef = $db->carregar($sql);
				continue;
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'A' and mt.metdetalhe='Ensino Médio' and metdesc ilike ('%reprovação%') and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
				$metas_aproveitamento_escolar_reprovacao_em = $db->carregar($sql);
				continue;
			}
		}
	}
	
	if($metas_aproveitamento_escolar_abandono_ef[0]) {
		foreach($metas_aproveitamento_escolar_abandono_ef as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	if($metas_aproveitamento_escolar_abandono_em[0]) {
		foreach($metas_aproveitamento_escolar_abandono_em as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	
	if($metas_aproveitamento_escolar_reprovacao_ef[0]) {
		foreach($metas_aproveitamento_escolar_reprovacao_ef as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	if($metas_aproveitamento_escolar_reprovacao_em[0]) {
		foreach($metas_aproveitamento_escolar_reprovacao_em as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	
	$arrTurmas = recuperaTurmasCriticasPorEscola();
	$arrTaxaReprovacaoBrasil = carregaTaxa();
	$arrDisciplinas = retornaDisciplinasTurma();
	$arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina();
	$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar();
	if($arrTurmas){
		if($arrTurmas['Ensino Fundamental']){
			foreach($arrTurmas['Ensino Fundamental'] as $em){
				if($arrDisciplinas[$em['pk_cod_turma']]){
					foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
						if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
							if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['U']){
								$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
								".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
								WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'C' and mt.metdetalhe='Ensino Fundamental' and mt.metstatus='A' 
								GROUP BY mt.mettipo, mt.metdesc, mt.metid 
								ORDER BY mt.metid";
								$metas_areas_conhecimento_ef = $db->carregar($sql);
								break;
							}
						}
					}
				}
			}
		}
		if($arrTurmas['Ensino Médio']){
			foreach($arrTurmas['Ensino Médio'] as $em){
				if($arrDisciplinas[$em['pk_cod_turma']]){
					foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
						if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
							if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['M']){
								$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
								".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
								WHERE mt.metmodulo = 'D' and mt.metsubmodulo = 'C' and mt.metdetalhe='Ensino Médio' and mt.metstatus='A' 
								GROUP BY mt.mettipo, mt.metdesc, mt.metid 
								ORDER BY mt.metid";
						
								$metas_areas_conhecimento_em = $db->carregar($sql);
								break;
							}
						}
					}
				}
			}
		}
	}
	
	if($metas_areas_conhecimento_ef[0]) {
		foreach($metas_areas_conhecimento_ef as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	if($metas_areas_conhecimento_em[0]) {
		foreach($metas_areas_conhecimento_em as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	 
	 return $metas ? $metas : false;
}


function listametas_diagnostico_3_ensinoeaprendizagem($dados) {
	global $db;
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_projetospedagogicos = $db->carregar($sql);
	if($existe_projetospedagogicos[0]) {
		
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
															        WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'E' and mt.metsubmodulo = 'P' and mt.metdetalhe='Projeto Pedagógico' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_projetospedagogicos = $db->carregar($sql);
		if($metas_projetospedagogicos[0]) {
			foreach($metas_projetospedagogicos as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
		
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_curriculo = $db->carregar($sql);
	if($existe_curriculo[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT COALESCE(rmetaxa::varchar,'') FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
															        WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'E' and mt.metsubmodulo = 'P' and mt.metdetalhe='Currículo' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid  
				ORDER BY mt.metid";
		
		$metas_curriculo = $db->carregar($sql);
		if($metas_curriculo[0]) {
			foreach($metas_curriculo as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_avaliacoes = $db->carregar($sql);
	if($existe_avaliacoes[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT COALESCE(rmetaxa::varchar,'') FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
																	WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'E' and mt.metsubmodulo = 'P' and mt.metdetalhe='Avaliações' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_avaliacoes = $db->carregar($sql);
		if($metas_avaliacoes[0]) {
			foreach($metas_avaliacoes as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_tempoaprendizagem = $db->carregar($sql);
	
	$sql = "SELECT * FROM pdeinterativo.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rtaporquecritico=TRUE";
	$respostatempoaprendizagem = $db->pegaLinha($sql);
	
	if($existe_tempoaprendizagem[0] || $respostatempoaprendizagem['rtacaso']=="N") {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT COALESCE(rmetaxa::varchar,'') FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
																	WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'E' and mt.metsubmodulo = 'T' and mt.metdetalhe='Tempo de Aprendizagem' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_tempoaprendizagem = $db->carregar($sql);
		if($metas_tempoaprendizagem[0]) {
			foreach($metas_tempoaprendizagem as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	return $metas;
	
	
}


function listaproblemas_diagnostico_3_ensinoeaprendizagem($dados) {
	global $db;
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_projetospedagogicos = $db->carregar($sql);
	if($pergs_projetospedagogicos[0]) {
		foreach($pergs_projetospedagogicos as $pergs) {
			$problemasensinoaprendizagem[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasensinoaprendizagem[$pergs['repid']]['codigo']) {
				$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasensinoaprendizagem[$pergs['repid']]['descricao']."', 'diagnostico_3_ensinoeaprendizagem') RETURNING papid;");
				$db->commit();
			}
			
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_curriculo = $db->carregar($sql);
	if($pergs_curriculo[0]) {
		foreach($pergs_curriculo as $pergs) {
			$problemasensinoaprendizagem[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasensinoaprendizagem[$pergs['repid']]['codigo']) {
				$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasensinoaprendizagem[$pergs['repid']]['descricao']."', 'diagnostico_3_ensinoeaprendizagem') RETURNING papid;");
				$db->commit();				
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_avaliacoes = $db->carregar($sql);
	if($pergs_avaliacoes[0]) {
		foreach($pergs_avaliacoes as $pergs) {
			$problemasensinoaprendizagem[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasensinoaprendizagem[$pergs['repid']]['codigo']) {
				$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasensinoaprendizagem[$pergs['repid']]['descricao']."', 'diagnostico_3_ensinoeaprendizagem') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_tempoaprendizagem = $db->carregar($sql);
	if($pergs_tempoaprendizagem[0]) {
		foreach($pergs_tempoaprendizagem as $pergs) {
			$problemasensinoaprendizagem[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasensinoaprendizagem[$pergs['repid']]['codigo']) {
				$problemasensinoaprendizagem[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasensinoaprendizagem[$pergs['repid']]['descricao']."', 'diagnostico_3_ensinoeaprendizagem') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rtaporquecritico=TRUE";
	$respostatempoaprendizagem = $db->pegaLinha($sql);
	
	$_PORQUE = array("espacofisico" => "Não possui espaço físico",
					 "profissionais" => "Não dispoe de profissionais para coordenar as atividades",
					 "recursosmateriais" => "Não dispoe de recursos materiais",
					 "outro" => "Outro");
	
	if($respostatempoaprendizagem['rtacaso']=="N") {
		$rtaporque = explode(";",$respostatempoaprendizagem['rtaporque']);
		if($rtaporque) {
			
			foreach($rtaporque as $pq) {
				$probl[] = $_PORQUE[$pq];	
			}
			$problemasensinoaprendizagem['rtaporque']['descricao'] = "A escola não desenvolve ações de educação integral porque: ".implode(", ",$probl);
			$problemasensinoaprendizagem['rtaporque']['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='rtaporque' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasensinoaprendizagem['rtaporque']['codigo']) {
				$problemasensinoaprendizagem['rtaporque']['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', 'rtaporque', 'A', '".$problemasensinoaprendizagem['rtaporque']['descricao']."', 'diagnostico_3_ensinoeaprendizagem') RETURNING papid;");
				$db->commit();				
			}
		}
	}
	
	return $problemasensinoaprendizagem;
	
}


function listaproblemas_diagnostico_4_gestao($dados) {
	global $db;
	$sql = "SELECT tp.tpeid, tp.tpedesc, count(p.pesid) as qtd FROM pdeinterativo.pessoa p 
			INNER JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
			LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
			LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
			LEFT JOIN pdeinterativo.tipoperfil tp ON tp.tpeid = ptp.tpeid 
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid IN(SELECT tpeid FROM pdeinterativo.perfilarea WHERE apeid IN('".APE_EQUIPEPEDAGOGICA."','".APE_DIRETOR."','".APE_VICEDIRETOR."','".APE_SECRETARIA."')) 
			AND p.critico=true
			GROUP BY tp.tpeid, tp.tpedesc";
	
	$pessoas = $db->carregar($sql);
	
	if($pessoas[0]) {
		foreach($pessoas as $pes) {
			$sql = "SELECT p.pesid FROM pdeinterativo.pessoa p 
					INNER JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid
					LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
					LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
					WHERE p.critico=true AND pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid='".$pes['tpeid']."'";
			
			$p = $db->carregar($sql);
			unset($pesids);
			if($p[0]) {
				foreach($p as $po) {
					$pesids[] = $po['pesid'];
				}
			}
			$problemasgestao['gestao_'.implode(",",$pesids)]['descricao'] = "Há ".$pes['qtd']." ".$pes['tpedesc']." que não possui(em) graduação.";
			$problemasgestao['gestao_'.implode(",",$pesids)]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='gestao_".implode(",",$pesids)."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao['gestao_'.implode(",",$pesids)]['codigo']) {
				$problemasgestao['gestao_'.implode(",",$pesids)]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', 'gestao_".implode(",",$pesids)."', 'A', '".$problemasgestao['gestao_'.implode(",",$pesids)]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_lideranca = $db->carregar($sql);
	if($pergs_lideranca[0]) {
		foreach($pergs_lideranca as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) {
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_acompanhamento = $db->carregar($sql);
	if($pergs_acompanhamento[0]) {
		foreach($pergs_acompanhamento as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) {
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_planejamento = $db->carregar($sql);
	
	if($pergs_planejamento[0]) {
		foreach($pergs_planejamento as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) {
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_rotinas = $db->carregar($sql);
	
	if($pergs_rotinas[0]) {
		foreach($pergs_rotinas as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) {
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_normas = $db->carregar($sql);
	
	if($pergs_normas[0]) {
		foreach($pergs_normas as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) { 
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_financeiras = $db->carregar($sql);
	
	if($pergs_financeiras[0]) {
		foreach($pergs_financeiras as $pergs) {
			$problemasgestao[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemasgestao[$pergs['repid']]['codigo']) {
				$problemasgestao[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemasgestao[$pergs['repid']]['descricao']."', 'diagnostico_4_gestao') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	return $problemasgestao;
}

function listametas_diagnostico_4_gestao($dados) {
	global $db;
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_direcao = $db->carregar($sql);
	if($existe_direcao[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '|| CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">')
																	 WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")."
				WHERE mt.metmodulo = 'G' and mt.metsubmodulo = 'D' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid  
				ORDER BY mt.metid";
		
		$metas_direcao = $db->carregar($sql);
		if($metas_direcao[0]) {
			foreach($metas_direcao as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_processos = $db->carregar($sql);
	if($existe_processos[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '|| CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
																	 WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")."		 
				WHERE mt.metmodulo = 'G' and mt.metsubmodulo = 'P' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_processos = $db->carregar($sql);
		if($metas_processos[0]) {
			foreach($metas_processos as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_financas = $db->carregar($sql);
	
	if($existe_financas[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '|| CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
																	 WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")."
				WHERE mt.metmodulo = 'G' and mt.metsubmodulo = 'F' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_financas = $db->carregar($sql);
		if($metas_financas[0]) {
			foreach($metas_financas as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	return $metas;
}


function listaproblemas_diagnostico_5_comunidadeescolar($dados) {
	global $db;
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Compromisso') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_compromisso = $db->carregar($sql);
	if($pergs_compromisso[0]) {
		foreach($pergs_compromisso as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Protagonismo e Participação') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_protagonismo = $db->carregar($sql);
	if($pergs_protagonismo[0]) {
		foreach($pergs_protagonismo as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace("(*)", $pergs['oppdesc'], $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Saúde e Bem-estar') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_saude = $db->carregar($sql);
	if($pergs_saude[0]) {
		foreach($pergs_saude as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Práticas') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_praticas = $db->carregar($sql);
	
	if($pergs_praticas[0]) {
		foreach($pergs_praticas as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Experiencia e Auto-Confiança') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_experiencia = $db->carregar($sql);
	
	if($pergs_experiencia[0]) {
		foreach($pergs_experiencia as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Cooperação e Respeito') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_cooperacao = $db->carregar($sql);
	if($pergs_cooperacao[0]) {
		foreach($pergs_cooperacao as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Motivação') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_motivacao = $db->carregar($sql);
	
	if($pergs_motivacao[0]) {
		foreach($pergs_motivacao as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Comunicação') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_comunicacao = $db->carregar($sql);
	
	if($pergs_comunicacao[0]) {
		foreach($pergs_comunicacao as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Participação') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$pergs_participacao = $db->carregar($sql);
	
	if($pergs_participacao[0]) {
		foreach($pergs_participacao as $pergs) {
			$problemascomunidadeescolar[$pergs['repid']]['descricao'] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
			$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$pergs['repid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar[$pergs['repid']]['codigo']) {
				$problemascomunidadeescolar[$pergs['repid']]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$pergs['repid']."', 'A', '".$problemascomunidadeescolar[$pergs['repid']]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	$sql = "SELECT rd.rdoid FROM pdeinterativo.respostadocente rd 
			INNER JOIN pdeinterativo.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
			WHERE rd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rd.rdocritico=TRUE AND rd.rdostatus='A'
			GROUP BY rd.rdoid";
	
	$docentesformacao = $db->carregarColuna($sql);
	
	if($docentesformacao) {
		$problemascomunidadeescolar['docentesformacao_'.implode(",",$docentesformacao)]['descricao'] = count($docentesformacao)." docentes consideram que sua formação não é apropriada para ministrar a área de conhecimento/ disciplina que atuam.";
		$problemascomunidadeescolar['docentesformacao_'.implode(",",$docentesformacao)]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='docentesformacao_".implode(",",$docentesformacao)."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		if(!$problemascomunidadeescolar['docentesformacao_'.implode(",",$docentesformacao)]['codigo']) {
			$problemascomunidadeescolar['docentesformacao_'.implode(",",$docentesformacao)]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', 'docentesformacao_".implode(",",$docentesformacao)."', 'A', '".$problemascomunidadeescolar['docentesformacao_'.implode(",",$docentesformacao)]['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
			$db->commit();
		}
		
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$respostapaiscomunidade = $db->pegaLinha($sql);
	
	if($respostapaiscomunidade) {
		if($respostapaiscomunidade['rpcpossuiconcelhocritico']=="t" && $respostapaiscomunidade['rpcpossuiconcelho']=="f") {
			$problemascomunidadeescolar['rpcpossuiconcelho']['descricao'] = "A escola não possui Conselho Escolar";
			$problemascomunidadeescolar['rpcpossuiconcelho']['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='rpcpossuiconcelho' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$problemascomunidadeescolar['rpcpossuiconcelho']['codigo']) {
				$problemascomunidadeescolar['rpcpossuiconcelho']['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', 'rpcpossuiconcelho', 'A', '".$problemascomunidadeescolar['rpcpossuiconcelho']['descricao']."', 'diagnostico_5_comunidadeescolar') RETURNING papid;");
				$db->commit();
			}
		}
	}
	
	return $problemascomunidadeescolar;
}

function listametas_diagnostico_5_comunidadeescolar($dados) {
	global $db;
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_estudantes = $db->carregar($sql);
	if($existe_estudantes[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '|| CASE WHEN mettipo='I' THEN REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') 
																	 WHEN mettipo='C' THEN REPLACE(metdesc,'K','<input type=checkbox  onclick=salvarFormularioParcialmente(); value=\"TRUE\" name=metas2['||mt.metid||'] '||COALESCE((SELECT CASE WHEN rmecheckbox=TRUE THEN 'checked' ELSE '' END FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'')||' >') END as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'C' and mt.metsubmodulo = 'E' and mt.metstatus='A' 
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
		
		$metas_estudantes = $db->carregar($sql);
		if($metas_estudantes[0]) {
			foreach($metas_estudantes as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}

	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_docentes = $db->carregar($sql);
	
	if($existe_docentes[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'C' and mt.metsubmodulo = 'D' and mt.metstatus='A'  
				GROUP BY mt.mettipo, mt.metdesc, mt.metid
				ORDER BY mt.metid";
		
		$metas_docentes = $db->carregar($sql);
		if($metas_docentes[0]) {
			foreach($metas_docentes as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A') AND
				  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";
	
	$existe_paiscomunidade = $db->carregar($sql);
	
	if($existe_paiscomunidade[0]) {
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'C' and mt.metsubmodulo = 'P' and mt.metstatus='A'  
				GROUP BY mt.mettipo, mt.metdesc, mt.metid
				ORDER BY mt.metid";
		
		$metas_paiscomunidade = $db->carregar($sql);
		if($metas_paiscomunidade[0]) {
			foreach($metas_paiscomunidade as $meta) {
				$metas[] = $meta['metdesc'];
			}
		}
	}
	
	return $metas;
}



function listaproblemas_diagnostico_6_infraestrutura($dados) {
	global $db;
	?>
	 <?php $arrInstalacoesNecessarias = recuperaInstalacoesNecessarias(); ?>
	 <?php $arrInstalacoesInadequadas = recuperaInstalacoesInadequadas(); ?>
	 <?php $arrEquipamentosRuins = recuperaEquipamentosRuins() ?>
	 <?php $arrRespEscola = false; ?>
	 <?php $arrRespEscola = recuperaRespostasEscola(null,"I","I",null,array("(op.oppdesc like 'Nunca' or op.oppdesc like 'Raramente')")); ?>
	 <?php if($arrInstalacoesNecessarias): ?>
		<?php foreach($arrInstalacoesNecessarias as $infra): ?>
			<?php if( $infra['rifcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola não possui ".$infra['ifidesc']."." ?>
				<?php $arrProblemas['codigo'][] = "naopossui_".$infra['ifiid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrInstalacoesInadequadas): ?>
		<?php foreach($arrInstalacoesInadequadas as $infra): ?>
			<?php if( $infra['rifcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola possui ". $infra['rifqtdinadequado'] ." ". $infra['ifidesc'] ." inadequados(as)." ?>
				<?php $arrProblemas['codigo'][] = "naopossui_".$infra['ifiid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrEquipamentosRuins): ?>
		<?php foreach($arrEquipamentosRuins as $equip): ?>
			<?php if( $equip['remcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola avalia que o estado de conservação de ".number_format($equip['rmeqtdruin'],"",2,".")." ".$equip['tmedesc']." é ruim." ?>
				<?php $arrProblemas['codigo'][] = "equipamento_".$equip['tmeid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	 <?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['codigo'][] = $resp['repid'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif;
	
	if($arrProblemas['descricao']){
		foreach($arrProblemas['descricao'] as $chave => $prob){
			$arrProblemasInfra[$chave]['descricao'] = $prob;
			$arrProblemasInfra[$chave]['codigo'] = $db->pegaUm("SELECT papid FROM pdeinterativo.planoacaoproblema WHERE papidentificador='".$arrProblemas['codigo'][$chave]."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			if(!$arrProblemasInfra[$chave]['codigo']){
				$arrProblemasInfra[$chave]['codigo'] = $db->pegaUm("INSERT INTO pdeinterativo.planoacaoproblema(pdeid, papidentificador, papstatus, papdescricao, abacod) VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$arrProblemas['codigo'][$chave]."', 'A', '".$prob."', 'diagnostico_6_infraestrutura') RETURNING papid;");
				$db->commit();
			}
		}	
	}
	return $arrProblemasInfra;
	
}

function listametas_diagnostico_6_infraestrutura($dados) {
	global $db;
	
	$arrInstalacoesNecessarias = recuperaInstalacoesNecessarias();
	if($arrInstalacoesNecessarias){
		foreach($arrInstalacoesNecessarias as $infra){
			if($infra['rifcritico'] == "t"){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'I' and mt.metsubmodulo = 'I' and mt.metstatus='A'  
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
				$metas_instalacoes = $db->carregar($sql);
				break;
			}
		}
	}else{
		$arrInstalacoesInadequadas = recuperaInstalacoesInadequadas();
		if($arrInstalacoesInadequadas){
			foreach($arrInstalacoesInadequadas as $infra){
				if($infra['rifcritico'] == "t"){
					$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
					".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
					WHERE mt.metmodulo = 'I' and mt.metsubmodulo = 'I' and mt.metstatus='A'  
					GROUP BY mt.mettipo, mt.metdesc, mt.metid 
					ORDER BY mt.metid";
					$metas_instalacoes = $db->carregar($sql);
					break;
				}
			}
		}
	}
	
	$arrEquipamentosRuins = recuperaEquipamentosRuins();
	if($arrEquipamentosRuins){
		foreach($arrEquipamentosRuins as $equip){
			if($equip['remcritico'] == "t"){
				$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||REPLACE(metdesc,'X','<input type=text onBlur=salvarFormularioParcialmente(); class=normal size=8 maxlength=8 value=\"'|| COALESCE((SELECT rmetaxa::varchar FROM pdeinterativo.respostameta rm WHERE rm.metid=mt.metid AND rm.pdeid=".$_SESSION['pdeinterativo_vars']['pdeid']." LIMIT 1),'') ||'\" name=metas['||mt.metid||'] onKeyUp=\"this.value=mascaraglobal(\'########\',this.value);\">') as metdesc FROM pdeinterativo.meta mt 
				".(($dados['somente_resposta'])?"INNER JOIN pdeinterativo.respostameta rm ON rm.metid=mt.metid AND (rm.rmetaxa IS NOT NULL OR rm.rmecheckbox=TRUE)":"")." 
				WHERE mt.metmodulo = 'I' and mt.metsubmodulo = 'E' and mt.metstatus='A'  
				GROUP BY mt.mettipo, mt.metdesc, mt.metid 
				ORDER BY mt.metid";
				$metas_equipamentos = $db->carregar($sql);
				break;
			}
		}
	}
	
	if($metas_instalacoes[0]) {
		foreach($metas_instalacoes as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	
	if($metas_equipamentos[0]) {
		foreach($metas_equipamentos as $meta) {
			$metas[] = $meta['metdesc'];
		}
	}
	
	return $metas? $metas : false;
}


function visualizarAnalises($dados) {
	global $db;
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	
	$sql = "SELECT cac.cacdesc, (SUM(COALESCE(pab.pabvalorcapital,0))+SUM(COALESCE(pab.pabvalorcusteiro,0))) as to, SUM(pab.pabvalorcapital) as ca, SUM(pab.pabvalorcusteiro) as cu 
				FROM pdeinterativo.planoacaoproblema pap 
				INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
				INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
				INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
				WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND ((pabfonte='P' AND pabparcela IN('S','P'))) AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' 
				GROUP BY cac.cacdesc";
	
	$cabecalho = array("Categoria","Valor Total","Capital","Custeio");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%',$par2);
	
	
	
}


function gerenciarAnalises($dados) {
	global $db;
	?>
	<html>
	<head>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script>
	function controlarIcone(obj, id) {
		if(obj.title=='menos') {
			obj.title='mais';
			obj.src='../imagens/mais.gif';
			document.getElementById(id).style.display='none';
		} else {
			obj.title='menos';
			obj.src='../imagens/menos.gif';
			document.getElementById(id).style.display='';
		}
	}
	</script>
	</head>
	<body>
	<form method="post" id="formulario2">
	<input type="hidden" name="requisicao" value="planoestrategico_0_3_visualizarplanoacao">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro">Analise de Preços e Categorias</td>
	</tr>
	<tr>
		<td>
		<p><b><img src="../imagens/menos.gif" style="cursor:pointer;" onclick="controlarIcone(this,'div_u_sup');" title="menos"> Itens com valores unitários superiores ao parâmetro do MEC</b></p>
		<div id="div_u_sup">
		<?
		
		$sql = "SELECT cac.cacdesc, cia.ciadesc, ure.uredesc, pab.pabqtd, pab.pabvalor, pab.pabvalorcapital, pab.pabvalorcusteiro,
					   CASE WHEN pabfonte='P' THEN 'PDDE/PDE Escola' ELSE 'Outras' END,
  					   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' ELSE '2ª Parcela (2012)' END as pabfonte,
		 			   ROUND(cia.ciaprecomaximo,2) as ciaprecomaximo, ROUND((((pab.pabvalor/cia.ciaprecomaximo)*100)-100),0) as porcent, '<input type=hidden name=planoacaobemservico[mec]['||pab.pabid||'] value=\"FALSE\"><input type=checkbox name=planoacaobemservico[mec]['||pab.pabid||'] value=\"TRUE\" '||CASE WHEN pabmecanalise=TRUE THEN 'checked' ELSE '' END||'>' as f 
				FROM pdeinterativo.planoacaoproblema pap 
				INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
				INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
				INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
				WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor > cia.ciaprecomaximo";
		
		$cabecalho = array("Categoria","Itens/Serviços","Unidade de referência",
						   "Quantidade","Valor Unitário(R$)","Capital(R$)","Custeio(R$)",
						   "Fonte","Parcela/Ano","Valor MEC","%","");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		?>
		</div>
		<p><b><img src="../imagens/menos.gif" style="cursor:pointer;" onclick="controlarIcone(this,'div_u_inf');"> Itens com valores unitários inferiores ao parâmetro do MEC</b></p>
		<div id="div_u_inf">
		<?
		
		$sql = "SELECT cac.cacdesc, cia.ciadesc, ure.uredesc, pab.pabqtd, pab.pabvalor, pab.pabvalorcapital, pab.pabvalorcusteiro,
					   CASE WHEN pabfonte='P' THEN 'PDDE/PDE Escola' ELSE 'Outras' END,
  					   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' ELSE '2ª Parcela (2012)' END as pabfonte,
		 			   ROUND(cia.ciaprecominimo,2) as ciaprecominimo, CASE WHEN pab.pabvalor > 0 THEN ROUND((((cia.ciaprecominimo/pab.pabvalor)*100)-100),0)::text ELSE '0' END as porcent, '<input type=hidden name=planoacaobemservico[mec]['||pab.pabid||'] value=\"FALSE\"> <input type=checkbox name=planoacaobemservico[mec]['||pab.pabid||'] value=\"TRUE\" '||CASE WHEN pabmecanalise=TRUE THEN 'checked' ELSE '' END||'>' as f 
				FROM pdeinterativo.planoacaoproblema pap 
				INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
				INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
				INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
				WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor < cia.ciaprecominimo";
		
		$cabecalho = array("Categoria","Itens/Serviços","Unidade de referência",
						   "Quantidade","Valor Unitário(R$)","Capital(R$)","Custeio(R$)",
						   "Fonte","Parcela/Ano","Valor MEC","%","");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		?>
		</div>
		<p><b><img src="../imagens/menos.gif" style="cursor:pointer;" onclick="controlarIcone(this,'div_u_inf');"> Categoria com valores superiores ao parâmetro do MEC</b></p>
		<div id="div_c_sup">
		<?
		
		$total = $db->pegaUm("SELECT SUM(pab.pabvalor) FROM pdeinterativo.planoacaobemservico pab 
					 		  INNER JOIN pdeinterativo.planoacaoacao paa ON pab.paaid = paa.paaid 
					 		  INNER JOIN pdeinterativo.planoacaoestrategia pae ON paa.paeid = pae.paeid 
					 		  INNER JOIN pdeinterativo.planoacaoproblema pap ON pae.papid = pap.papid 
					 		  INNER JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid
							  INNER JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
							  INNER JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
					 		  WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		$sql = array();
		if($total) {
			$sql = "SELECT cac.cacdesc, SUM(pab.pabvalor) as pabvalor, ROUND((SUM(pab.pabvalor)/".$total.")*100,2), cac.cacpercentual, '<input type=checkbox name=categoriaacaoanalise[] value='||cac.cacid||' '|| CASE WHEN caa.caaid IS NOT NULL THEN 'checked' ELSE '' END ||'>' as f 
					FROM pdeinterativo.planoacaoproblema pap 
					INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
					INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
					INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
					INNER JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
					INNER JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
					INNER JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid 
					LEFT JOIN pdeinterativo.categoriaanalise caa ON caa.cacid = cac.cacid AND caa.pdeid = pap.pdeid 
					WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'
					GROUP BY cac.cacid, cac.cacdesc, cac.cacpercentual, caa.caaid 
					HAVING ROUND((SUM(pab.pabvalor)/".$total.")*100,2) > cac.cacpercentual";
		}
		
		$cabecalho = array("Categoria","Total(R$)","Percentual Atingido", "% MEC","");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		?>
		</div>


		</td>
	</tr>
	<tr>
	<td class="SubTituloCentro"><input type="submit" value="Salvar"> <input type="button" value="Fechar" onclick="window.close();"></td>
	</tr>
	</table>
	</form>
	</body>
	</html>
	<?
}


function planoestrategico_0_3_visualizarplanoacao($dados) {
	global $db;
	
	apagarCachePdeInterativo();
	
	if($dados['planoacaobemservico']) {
		foreach($dados['planoacaobemservico'] as $tipo => $arr) {
			foreach($arr as $pabid => $bollean) {
				
				$sql = "UPDATE pdeinterativo.planoacaobemservico
					    SET pab".$tipo."analise=".$bollean."
						WHERE pabid='".$pabid."'";
				
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	
	$db->executar("DELETE FROM pdeinterativo.categoriaanalise WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if($dados['categoriaacaoanalise']) {
		foreach($dados['categoriaacaoanalise'] as $cacid) {
			$db->executar("INSERT INTO pdeinterativo.categoriaanalise(pdeid, cacid)
    					   VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$cacid."');");
			$db->commit();
		}
	}
	

		
	echo "<script>
			alert('Dados gravados com sucesso');
			".(($dados['togo'])?"window.location='".$dados['togo']."';":"window.close();")."
		  </script>";
}

function condicaoComite() {
	global $db;

	if($_SESSION['pdeinterativo_vars']['pditempdeescola']=="f") {
		return "Escolas Sem PDE Interativo não podem tramitar o plano";
	}
	
	if(!$_SESSION['pdeinterativo_vars']['pdeid']) {
		return "Ocorreu um erro: Não foi possível identificar a escola, clique na Tela Inicial e recomçe a navegação.";
	}
	
	$num = $db->pegaUm("SELECT COUNT(abrid) as num FROM pdeinterativo.abaresposta WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if($num < 32) $erro .= "Progresso de Preenchimento do PDE não esta 100%;".'\n';
	
	$sql = "select count(*) as num from pdeinterativo.aba a 
			inner join pdeinterativo.abaresposta b on b.abaid = a.abaid 
			where b.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' and a.abacod='planoestrategico_0_2_planoacao'";
	
	$num = $db->pegaUm($sql);
	
	if($num==0) {
		$erro .= "- Possivelmente o plano foi atualizado, clique na aba 1.2 Planos de ação e clique novamente no botão Salvar;".'\n';
	}
	
	$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE codinep='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND cccstatus='A'";
	$cargacapitalcusteio = $db->pegaLinha($sql);
	$problema = false;
	
	$_parcelas = array('P' => 'primeira',
					   'S' => 'segunda');

	foreach($_parcelas as $parcela => $descricao) {
		
		$outros_pabs = $db->pegaLinha("SELECT COALESCE(SUM(pabvalorcapital),0) as pabvalorcapital, COALESCE(SUM(pabvalorcusteiro),0) as pabvalorcusteiro 
								  	   FROM pdeinterativo.planoacaobemservico p 
								  	   INNER JOIN pdeinterativo.planoacaoacao po ON po.paaid = p.paaid 
								  	   INNER JOIN pdeinterativo.planoacaoestrategia pe ON pe.paeid = po.paeid 
								  	   INNER JOIN pdeinterativo.planoacaoproblema pp ON pp.papid = pe.papid  
								  	   WHERE pabparcela='".$parcela."' AND 
	   										 pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
								  	   		 pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A'");

		
		if($outros_pabs['pabvalorcapital'] != $cargacapitalcusteio['ccccapital'.$descricao])
			$financas[]="- ".ucfirst($descricao)." parcela de capital ainda possui recursos;".'\n';
		if($outros_pabs['pabvalorcusteiro'] != $cargacapitalcusteio['ccccusteio'.$descricao])
			$financas[]="- ".ucfirst($descricao)." parcela de custeio ainda possui recursos;".'\n';
			
		
	}
	
	if($financas) {
		$erro .= "Ainda há recursos disponíveis a serem utilizados:".'\n'.implode("",$financas);
	}
	
	if($erro) {
		return $erro;
	} else {
		return true;
	}
	
}

function condicaoMEC() {
	global $db;
	
	$sql = "SELECT COUNT(prcid) FROM pdeinterativo.parecer WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' ANd prcstatus='A' AND prcaprovado=TRUE";
	$pareceres = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(pabid) FROM pdeinterativo.planoacaoproblema pr 
			LEFT JOIN pdeinterativo.planoacaoestrategia pe ON pe.papid = pr.papid 
			LEFT JOIN pdeinterativo.planoacaoacao pa ON pa.paeid = pe.paeid 
			LEFT JOIN pdeinterativo.planoacaobemservico ps ON ps.paaid = pa.paaid 
			WHERE pabcomiteanalise=FALSE AND papstatus='A' AND pabstatus='A' AND paestatus='A' AND paastatus='A' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$acoesnaomarcadas = $db->pegaUm($sql);
	
	if($pareceres==6 && $acoesnaomarcadas==0) {
		return true;
	} else {
		return false;
	}
}

function condicaoValidado() {
	global $db;
	
	if($_SESSION['pdeinterativo_vars']['pditempdeescola']=="f") {
		return "Escolas Sem PDE Interativo não podem tramitar o plano";
	}
	
	$sql = "SELECT pabmecanalise 
			FROM pdeinterativo.planoacaoproblema pap 
			INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
			INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
			INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
			LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
			LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
			LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
			WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor > cia.ciaprecomaximo";
	
	$mecanalisemaximo = $db->carregarColuna($sql);
			
	$sql = "SELECT pabmecanalise 
			FROM pdeinterativo.planoacaoproblema pap 
			INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
			INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
			INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
			LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
			LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
			LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
			WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor < cia.ciaprecominimo";
	
	$mecanaliseminimo = $db->carregarColuna($sql);

	$ima = false;
	if($mecanalisemaximo) {
		foreach($mecanalisemaximo as $ia) {
			if($ia != "t") $ima=true;
		}
	}
	
	if($ima) $erro.="Itens com valores unitários superiores ao parâmetro do MEC não foram validados;".'\n';
	
	$imo = false;
	if($mecanaliseminimo) {
		foreach($mecanaliseminimo as $io) {
			if($io != "t") $imo=true;
		}
	}
	
	if($imo) $erro.="Itens com valores unitários inferiores ao parâmetro do MEC não foram validados;".'\n';
	
	if($erro) return $erro;
	else return true;
	

}

function validarPdeInterativo()
{
	global $db;
	/* configurações do relatorio - Memoria limite de 1024 Mbytes */
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	/* FIM configurações - Memoria limite de 1024 Mbytes */
	
	include_once("pdeWs.php");
	$ws = new pdeWs();
	
	//$entcodent = $db->pegaUm("select pdicodinep from pdeinterativo.pdinterativo where entid = ".$_SESSION['entid']);
	if($_SESSION["pdeinterativo_vars"]["pdicodinep"]) {
		$entcodent = $_SESSION["pdeinterativo_vars"]["pdicodinep"];
		
		$coProgramaFNDE = 96;
		$anoAtual = date('Y');
	
		$teste = $ws->pdeEscolaWs('atualizaAnaliseEscola', $anoAtual, $entcodent, $coProgramaFNDE);
		
		if( $teste && $teste != "errowebservice" )
		{
			$db->executar("UPDATE pdeinterativo.pdinterativo SET pdiretornofnde='t' WHERE pdistatus = 'A' and pdicodinep = '{$entcodent}'");
			$db->commit();
			return true;
		}
		else
		{
			$db->executar("UPDATE pdeinterativo.pdinterativo SET pdiretornofnde='f' WHERE pdistatus = 'A' and pdicodinep = '{$entcodent}'");
			$db->commit();
			return false;
		}
		return true;
	} else {
		return false;
	}
}

?>