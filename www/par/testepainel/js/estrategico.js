jQuery(document).ready(function() {
    // Radialize the colors
    Highcharts.getOptions().colors = Highcharts.map(
            [         '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro
                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro
                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro
                    , '#008000' // Verde
                    , '#CD0000' // Vermelho
                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
        ]
        , function(color) {
			return {
				radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
				stops: [
					[0, color],
					[1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
				]
			};
		})
});


function refreshAutomatico() {
	setTimeout("location.reload(true);",3600000);
}

function atualizaUsuario(){
	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "useronline=1",
		success: function(msg){
		jQuery('#usuOnline').html( msg );
		}
	});
	window.setTimeout('atualizaUsuario()', 5000);
}
function abreUsuarios(sisid){
	window.open(
		'../geral/usuarios_online2.php?sisid='+sisid,
		'usuariosonline',
		'height=500,width=600,scrollbars=yes,top=50,left=200'
	);
}
function buscar(busca){
	window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
}
function abreIndicadores(atiprojeto, atiidraiz){
	window.open('/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A&atiprojeto='+atiprojeto+'&atiidraiz='+atiidraiz,'Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}
function acessarMaisEducacao(entid,memid){
	url = "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A&painel=1&entid=" + entid + "&memid=" + memid;
	window.open(url,'Mais Educação','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}
function abreRelatorioFinanceiro(vano, vsubacao){
	window.open('/financeiro/financeiro.php?modulo=relatorio/geral_teste&acao=R&painel=1&submetido=1&ano='+vano+'&escala=1&agrupador[0]=subacao&agrupador[1]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&subacao[0]='+vsubacao+'&alterar_ano=0','Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}
function abreIndicadorPopUp(indid){
	var url = "../painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=" + indid + "&abreMapa=1&cockpit=1";
	window.open(url,'Indicador','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}
function abreAlinhamentoEstrategico(area, eixo, principal, superior, tipo)
{
	   window.open('estrategico.php?modulo=principal/alinhamento_estrategico&acao=A&chamadaExterna=1&area='+area+'&eixo='+eixo+'&qdPrincipal='+principal+'&qdSuperior='+superior+'&tipo='+tipo);
}
function acessarObras(obrid)
{
	url = "/obras/obras.php?modulo=principal/cadastro&acao=A&painel=1&obrid=" + obrid;
	window.open(url,'Obras','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}
function abreRelatorioObras(orgid, filtroagrupador, prfid, tooid, stoid) {
	window.open('/obras/obras.php?modulo=relatorio/relatorio_geral&acao=A&orgid=' + orgid + '&filtroagrupador=' + filtroagrupador +'&prfid=' + prfid +'&tooid=' + tooid +'&stoid=' + stoid, '_blank');
}

//Aguardando para retirar
function abreRelatorio(params)
{
	window.open('/financeiro/financeiro.php?modulo=relatorio/geral_teste&acao=R&'+params,'Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function abreRedeFederal(rede)
{
	var url = '/academico/academico.php?modulo=principal/mapaSupProf&acao='+rede+'&cockpit=1';
	window.open(url,'Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function acessarObras(obrid)
{
	url = "/obras/obras.php?modulo=principal/cadastro&acao=A&painel=1&obrid=" + obrid;
	window.open(url,'Obras','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function abreRelatorioEnem(params)
{
	window.open('/pde/enem.php?modulo=relatorio_enem/relatorio_checklist&acao=A&'+params,'Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function abreRelatorioRedeFederal(vfase, vsituacao, vinstalacao){
	window.open('/academico/academico.php?modulo=relatorio/download&acao=A&painel=1&exiid='+vfase+'&cmpsituacao='+vsituacao+'&cmpinstalacao='+vinstalacao,'Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function acessarPainelGerenciamento(painel_estrategico,situacao,ano) {
	window.open('/par/par.php?modulo=principal/painelGerenciamento&acao=A&painel_estrategico='+painel_estrategico+'&situacao='+situacao+'&ano='+ano, '_blank');	
}

function abreRelatorioObrasCockpit(quadro, prfid, tooid, stoid){
	window.open('/pde/estrategico.php?modulo=principal/popuplistaobras&acao=A&quadro='+quadro+'&prfid='+prfid+'&tooid='+tooid+'&tipo='+stoid);
}

function abreAcaoPainel(acaid){
	window.open('/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='+acaid);
}