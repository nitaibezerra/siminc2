<?

if($_SESSION['memid']){
	if($_REQUEST['salvaMsg']){
		
		$sql = "UPDATE pdeescola.memaiseducacao
				   SET memorientacao='".$_REQUEST['chk1']."', 
				   memorientacaocpf='".$_SESSION['usucpf']."', 
				   memmostrarnova='".$_REQUEST['chk2']."'
				 WHERE memid=".$_SESSION['memid'];
		$db->executar($sql);
		$db->commit();
		echo 'OK';
		die();
	}
	
	$sql = "select memorientacao, memmostrarnova from pdeescola.memaiseducacao
			WHERE memid=".$_SESSION['memid'];
	$dados = $db->pegaLinha($sql);
	if($dados) extract($dados);
}

header('content-type: text/html; charset=ISO-8859-1');

?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
		<script type="text/javascript" src="/includes/prototype.js"></script>
	</head>
	<body>
		<center>
			<div style="font-size:12px" >
					<center><b><font color=red>NOVIDADES IMPORTANTES</font></b></center>
					<br>
					
						<div style="margin: 0 auto; padding: 0; height: 420px; width: 100%; border: none;" class="div_rolagem">
						
						<p class=MsoListParagraphCxSpFirst style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>O redesenho do PME (Escolas Urbanas) em 2013</span></b><span
						style='font-size:10.0pt;line-height:115%'> responde ao desafio de superar a dicotomia entre turno e contra-turno de modo que as atividades escolhidas sejam incorporadas nas práticas cotidianas dos professores e professoras. Desta forma a reorganização do Macrocampo Acompanhamento Pedagógico foi elaborada na perspectiva de equilibrar a agenda curricular de modo que a escola vislumbre a organização do tempo integral, articulando as atividades do PME e os componentes curriculares. <b style='mso-bidi-font-weight:normal'>As escolas do
						campo permanecem com o mesmo desenho e regras de 2012.<o:p></o:p></b></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%'>A partir de 2013 o PME contemplará atividades específicas para <b style='mso-bidi-font-weight:normal'>jovens de <st1:metricconverter
						ProductID="15 a" w:st="on">15 a</st1:metricconverter> 17 anos</b> que ainda estejam no ensino fundamental. O objetivo é proporcionar a estes estudantes um espaço educativo para aprendizagens e convivência, e assegurar sua permanência para a conclusão com qualidade do Ensino Fundamental.<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpLast style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%'>Escolas que compõem o <b
						style='mso-bidi-font-weight:normal'>Pacto Nacional da Alfabetização</b> na idade certa devem contemplar estudantes dos anos iniciais nas atividades do PME envolvendo a <u>atividade de Acompanhamento Pedagógico, especificamente com ações de alfabetização e letramento.<o:p></o:p></u></span></p>
						
						<p class=MsoListParagraphCxSpFirst style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%;mso-font-kerning:1.5pt;mso-fareast-language:
						PT-BR'>Para garantir a efetividade do Programa Mais Educação e a conexão das atividades do PME ao projeto político pedagógico da escola, em 2013 o programa passará a ofertar às escolas urbanas <b style='mso-bidi-font-weight:normal'>sete
						<span class=SpellE>Macrocampos</span></b>, sendo eles: 1) Acompanhamento Pedagógico 2) Comunicação uso de Mídias, Cultura Digital e Tecnológica 3) Cultura, Artes e Educação Patrimonial; 4) Educação Ambiental e Sociedade Sustentável; 5) Esporte e Lazer; 6) Educação em Direitos Humanos e 7) Promoção da Saúde. As atividades dos macrocampos que deixam de existir foram incorporadas aos atuais macrocampos.</span><span
						style='font-size:10.0pt;line-height:115%'><o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>O <span class=SpellE>Macrocampo</span> Acompanhamento Pedagógico</span></b><span
						style='font-size:10.0pt;line-height:115%'> continua sendo obrigatório, agora com apenas uma atividade que contemplará as diferentes áreas do conhecimento envolvendo todas as atividades disponíveis anteriormente (alfabetização, matemática, história, ciências, geografia e línguas estrangeiras). <b
						style='mso-bidi-font-weight:normal'>Essa atividade será denominada, Orientação
						de Estudos e Leitura</b> e tem por objetivo a articulação entre o currículo estabelecido da escola e as atividades pedagógicas propostas pelo PME, ensejando assim o permanente diálogo entre os professores da escola e os monitores que atuam no PME. Assim esta atividade deverá ser realizada com duração de uma hora à uma hora e meia, diariamente, sendo mediada por um monitor orientador de estudos, que seja preferencialmente um estudante de graduação  ou das Licenciaturas vinculado ao PIBID (Programa Institucional de Bolsa de Iniciação à Docência), ou estudantes de graduação com estágio supervisionado.<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%;
						text-autospace:ideograph-numeric'><span style='font-size:10.0pt;line-height:
						115%;mso-font-kerning:1.5pt;mso-fareast-language:PT-BR'>As atividades dos <span
						class=SpellE>macrocampos</span> “<b style='mso-bidi-font-weight:normal'>Educação
						<st1:PersonName ProductID="em Direitos Humanos" w:st="on">em Direitos Humanos</st1:PersonName></b>”
						e <b style='mso-bidi-font-weight:normal'>“Promoção da Saúde</b>”, quando escolhidas, devem ser articuladas como temas transversais nas demais atividades selecionadas no PME, sobretudo nos Macrocampos de Comunicação uso de Mídias, Cultura Digital e Tecnológica (ex. jornal, rádio escolar e etc) e Cultura, Artes e Educação Patrimonial (ex. teatro, dança e etc), considerando que a partir de agora, para estas duas atividades, serão disponibilizados recursos de custeio e capital para o desenvolvimento destas temáticas. A proposta é estimular nos estudantes a reflexão e o diálogo sobre seus direitos e responsabilidades enquanto protagonistas de uma sociedade livre, pluralista e inclusiva, a partir do contexto escolar e social no qual estão inseridos.  <o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%;
						text-autospace:ideograph-numeric'><span style='font-size:10.0pt;line-height:
						115%;mso-font-kerning:1.5pt;mso-fareast-language:PT-BR'>O PME traz as atividades olímpicas de: badminton, luta olímpica e vôlei de praia, além disso o PST transformou-se na atividade “Esporte na Escola/Atletismo e Múltiplas Vivências Esportivas”( basquete, futebol, futsal, handebol, voleibol e xadrez), que objetiva qualificar a ação pedagógica, por meio de uma proposta planejada, inclusiva, participativa, que possibilita o desenvolvimento de diversas modalidades, tendo o atletismo como base, e valoriza o prazer e o lúdico, fundamentada nos pressupostos do Esporte Educacional e articulada com o projeto pedagógico da Escola. Essa atividade deverá ser realizada 2 vezes por semana e cada encontro com duração mínima de uma hora, sendo mediada por um monitor, que seja preferencialmente um estudante de graduação da área da Educação Física ou Esporte, vinculado ao PIBID (Programa Institucional de Bolsa de Iniciação à Docência), ou a outro estágio supervisionado.</span><span style='font-size:10.0pt;line-height:115%;
						mso-font-kerning:1.5pt;mso-fareast-language:PT-BR'><o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%;
						text-autospace:ideograph-numeric'><span style='font-size:10.0pt;line-height:
						115%;mso-font-kerning:1.5pt;mso-fareast-language:PT-BR'>Para desenvolver as atividades, a escola receberá um kit de material esportivo diversificado, recurso para aquisição de kit de material específico para o atletismo e um kit de material didático-pedagógico, além de capacitação para os monitores e acompanhamento das ações.<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%'>Clique aqui caso deseje obter mais
						informações sobre o Esporte na Escola:<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><span
						class=GramE><b style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;
						line-height:115%'>http://</span></b></span><b style='mso-bidi-font-weight:normal'><span
						style='font-size:10.0pt;line-height:115%'></span></b><span style='font-size:
						10.0pt;line-height:115%'><a target="_blank"
						href="http://www.esporte.gov.br/snelis/segundotempo/maiseducacao/default.jsp"><b
						style='mso-bidi-font-weight:normal'>www.esporte.gov.br/snelis/segundotempo/maiseducacao/default.jsp</b></a><b
						style='mso-bidi-font-weight:normal'><o:p></o:p></b></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%;mso-font-kerning:1.5pt;mso-fareast-language:
						PT-BR'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>CADASTRO NO SIMEC<o:p></o:p></span></b></p>
						
						<p class=MsoListParagraphCxSpLast style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>Atividades 2013 </span></b><span style='font-size:10.0pt;line-height:
						115%'>- Neste novo desenho a escola deverá optar pela atividade Orientação de Estudos e Leitura (obrigatória) e mais 3 atividades correspondentes aos demais Macrocampos. 
Caso a escola queira a 5° atividade será obrigatoriamente a atividade: Esporte na Escola/ Atletismo e múltiplas vivências esportivas.
<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='margin-left:0.0pt;text-align:justify;line-height:
						10.0pt'><b><span style='font-size:10.0pt;color:black;mso-font-kerning:12.0pt'>Relação
						Escola-Comunidade: </span></b><span style='font-size:10.0pt;color:black;
						mso-font-kerning:12.0pt;mso-bidi-font-weight:bold'>incentiva e apóia a abertura das escolas nos finais de semana proporcionando o desenvolvimento de diversas oficinas com intuito de fortalecer a convivência comunitária, evidenciar a cultura popular, as expressões juvenis e o protagonismo da comunidade, além de contribuir para valorizar o território e os sentimentos de identidade e pertencimento.<b> A aba “Relação
						Escola-Comunidade&quot; será disponibilizada caso a escola opte pela abertura
						aos finais de semana,</b> momento em que deverão ser informadas as oficinas temáticas de interesse da sua comunidade. Para o desenvolvimento desta ação, serão destinados recursos de custeio e capital proporcionais aos estudantes declarados no Educacenso, destinado ao ressarcimento de oficineiros voluntários e aquisição de materiais para o desenvolvimento das oficinas.</span><span
						style='font-size:10.0pt'><o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><b><span
						style='font-size:10.0pt;line-height:115%;mso-font-kerning:12.0pt'>Jovens de <st1:metricconverter
						ProductID="15 a" w:st="on">15 a</st1:metricconverter> 17 anos: </span></b><span
						style='font-size:10.0pt;line-height:115%;mso-font-kerning:12.0pt;mso-bidi-font-weight:
						bold'>Esta ação visa construir propostas de atividades com os jovens, que propiciem trabalhos integrados entre diferentes áreas de conhecimento tendo como objetivo principal o de orientar a criação de espaço para pensar seu projeto de vida desenvolvendo: AUTORIA e AUTONOMIA do grupo de estudantes. Além disso, espera-se que tais atividades permitam aos jovens lançar um olhar sobre suas trajetórias escolares, planejando e executando propostas de caráter investigativo, bem como de organização de ações que lhe permitam prosseguir em seus estudos e realizar aproximações com o mundo do trabalho. Para o desenvolvimento da proposta, serão disponibilizados: materiais pedagógicos de apoio; recursos de custeio e capital; ressarcimento de  <b style='mso-bidi-font-weight:
						normal'>monitor tutor</b> responsável por acompanhar este grupo de estudantes.<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><span
						style='font-size:10.0pt;line-height:115%;color:black;mso-font-kerning:12.0pt;
						mso-bidi-font-weight:bold'>Assim, a <b>aba “Jovens de <st1:metricconverter
						ProductID="15 a" w:st="on">15 a</st1:metricconverter> 17 anos&quot; será
						disponibilizada caso a escola opte pelo desenvolvimento desta atividade
						específica</b>, devendo informar o número de estudantes participantes da ação. <o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>Espaços PME:</span></b><span style='font-size:10.0pt;line-height:115%'> <b
						style='mso-bidi-font-weight:normal'>esta aba é de preenchimento obrigatório </b><span
						style='mso-bidi-font-weight:bold'>e destinado à inserção de fotos e arquivos que mostrem os diferentes espaços (dentro e fora da escola) onde ocorrem as atividades do Programa Mais Educação. No caso de escolas que já fizeram modificações em suas estruturas por meio de recursos do PME, é importante postar imagens que mostrem o antes e o depois.</span><o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;line-height:115%'><b
						style='mso-bidi-font-weight:normal'><span style='font-size:10.0pt;line-height:
						115%'>Questionário Monitoramento Físico-Financeiro:</span></b><span
						style='font-size:10.0pt;line-height:115%'> <b style='mso-bidi-font-weight:normal'>esta
						aba é de preenchimento obrigatório.</b> No item 1 - Recursos PDDE/Educação Integral da escola deverá especificar os recursos referentes as rubricas de custeio e capital a partir do saldo informado pelo FNDE.<o:p></o:p></span></p>
						
						<p class=MsoListParagraphCxSpMiddle style='text-align:justify;margin-left:0.0pt'><b style='mso-bidi-font-weight:
						normal'><span style='font-size:10.0pt'>Tramitação do Plano de Atendimento: </span></b><span
						style='font-size:10.0pt'>Sua escola acaba de sair do status não iniciado para o status em cadastramento e permanecerá assim até o preenchimento de todas as abas do seu plano de atendimento. Após o término do preenchimento do plano é necessário que você abra a aba pendências para enviar o plano para a secretaria municipal/estadual. Basta clicar no botão, na lateral direita da tela, enviar para avaliação na secretaria municipal/estadual.</span></p>
						
						</div>		
					
					
					<br>
					<table align="center">
						<tr>
							<td>
								<input type='checkbox' name='memorientacao' id='memorientacao' onclick="liciente();" <?if($memorientacao == 't') echo 'checked';?>>
								<b>Li e estou ciente do PME 2013</b>
								
								<span id="btn_fechar" style="display: <?if($memorientacao != 't') echo 'none';?>" align="right">
									&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
									<input type="button" value="Fechar Janela" onclick="closeMessage();" />
								</span>
								
								<br><br> 
								<input type='checkbox' name='memmostrarnova' id='memmostrarnova' onclick="liciente();" <?if($memmostrarnova == 't') echo 'checked';?>>
								<b>Deseja não visualizar essa mensagem novamente?</b>
								
							</td>
						</tr>
					</table>	
									
		 		  </div>
			<br />
			
		</center>
	</body>
</html>	



<script>
	function liciente(){
		
		var valor1 = '';
		var valor2 = '';
		
		if(document.getElementById('memorientacao').checked == true){
			valor1 = 't';
		}else{
			valor1 = 'f';
		}
		
		if(document.getElementById('memmostrarnova').checked == true){
			valor2 = 't';
		}else{
			valor2 = 'f';
		}
		
		var myAjax = new Ajax.Request(
			"pdeescola.php?modulo=meprincipal/alertDadosEscola&acao=A",
			{
				method: 'post',
				parameters: "salvaMsg=OK&chk1=" + valor1 + "&chk2=" + valor2,
				asynchronous: false,
				onComplete: function(resp) {
					
					if(resp.responseText == 'OK') {
						if(document.getElementById('memorientacao').checked == true){
							document.getElementById('btn_fechar').style.display = '';
						}
						else{
							document.getElementById('btn_fechar').style.display = 'none';
						}
					} 
					
					
				}
			});
	}
</script>