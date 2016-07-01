
function retornaMes(mes)
{
    switch(mes){
        case '1':
            return 'Janeiro';
        case '2':
            return 'Fevereiro';
        case '3':
            return 'Março';
        case '4':
            return 'Abril';
        case '5':
            return 'Maio';
        case '6':
            return 'Junho';
        case '7':
            return 'Julho';
        case '8':
            return 'Agosto';
        case '9':
            return 'Setembro';
        case '10':
            return 'Outubro';
        case '11':
            return 'Novembro';
        case '12':
            return 'Dezembro';
    }
}

function retornaCor(val)
{
    switch(val){
        case '1':
            return '#337ab7';
        case '2':
            return '#5cb85c';
        case '3':
            return '#5bc0de';
        case '4':
            return '#f0ad4e';
        case '5':
            return '#d9534f';
        case '6':
            return '#A9E8C4';
        case '7':
            return '#E470D4';
        case '8':
            return '#E419370';
        case '9':
            return '#5A8E6B';
        case '10':
            return '#5A708E';
    };
}

function verificaIndexTable(index, cor)
{
    var span = $($('#calendario tr td')[index]).find('span');
    $($('#calendario tr td')[index]).css('background-color',cor);
    if($($(span)[0]).html() != undefined){
        if($($(span)[1]).html() != undefined){
            if($($(span)[1]).attr('data-pos') == 'final'){
                return;
            }
        }
        if($($(span)[0]).attr('data-pos') == 'final'){
            return;
        }
    }
    verificaIndexTable(index+1, cor);
}